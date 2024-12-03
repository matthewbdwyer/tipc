"""
Python executable with optional commandline parameters to filter tests

Run `python3 run_system_tests.py --help` for help
Run `python3 run_system_tests.py --serial` to disable parallel execution

Example usage:
    mst3k@portal06 ~/tipc/test/system $ python3 run_system_tests.py
    Running 102 tests...
    PASSED: selftests/addrof.tip  (self-contained, optimized)
    PASSED: selftests/ptr2.tip  (self-contained, unoptimized)
    ...
    FAILED: iotests/linkedlist.tip  (unwritable output)
    No error occurred when executing command: /u/fpv5gr/CS_4620/tipc/build/src/tipc iotests/linkedlist.tip --pa=/tmp/tmpeudkhscm/unwritable
    PASSED: iotests/linkedlist.tip  (unwritable output)

    1/102 tests failed
    Failed tests:
    - iotests/linkedlist.tip  (unwritable output)

    mst3k@portal06 ~/tipc/test/system $ python3 run_system_tests.py -k error
    Running 2 tests...
    PASSED: iotests/parseerror.tip  (self-contained, optimized, expected to fail)
    PASSED: iotests/semanticerror.tip  (self-contained, optimized, expected to fail)

    All 2 tests passed

    mst3k@portal06 ~/tipc/test/system $ python3 run_system_tests.py -d polytests
    Running 2 tests...
    PASSED: polytests/ident.tip  (pretty-print)
    PASSED: polytests/apply.tip  (pretty-print)

    All 2 tests passed

How to extend:
    To add more tests, add more tuples to the `test_categories` list in the `collect_tests` function.
    See the comment in that function for more info.
"""

import argparse
import os
import subprocess
import tempfile
import difflib
from pathlib import Path
from colorama import Fore, Style
from functools import wraps
import concurrent.futures
import random

ROOT_DIR = os.getenv('GITHUB_WORKSPACE', subprocess.getoutput('git rev-parse --show-toplevel'))
TIPC = f"{ROOT_DIR}/build/src/tipc"
RTLIB = f"{ROOT_DIR}/rtlib"
TIPCLANG = os.getenv('TIPCLANG')

class TestData:
    def __init__(self, path, directory, test_func, info=""):
        self.path = path
        self.directory = directory
        self.test_func = test_func
        self.info = info
        self.failure_output = None

    def run(self) -> bool:
        """Executes the test using the provided function. Stores output if it fails."""
        try:
            success, output = self.test_func(self.path)
        except TestExecutionError as e:
            success = False
            output = str(e)
        if not success:
            self.failure_output = output
        return success
        
    def __str__(self):
        if not self.info:
            self.info = self.test_func.__doc__
        return f"{self.path}  ({self.info})"

class CompileError(Exception):
    def __init__(self, message):
        super().__init__(message)

class TestExecutionError(Exception):
    def __init__(self, message):
        super().__init__(message)

def run_command(command: str, check=False):
    """Run a shell command and return its exit code and output."""
    try:
        result = subprocess.run(command, shell=True, capture_output=True, text=True, check=check)
    except subprocess.CalledProcessError as e:
        raise TestExecutionError(f"Error occurred when executing: {command}\nStderr:\n{e.stderr}")
    return result.returncode == 0, result.stdout + result.stderr

def compile_file(file: Path, tmp: Path, flags="") -> str:
    """Compile a .tip file and return the executable full path."""
    bc_file = tmp / f"{file.stem}.bc"
    executable_file = tmp / file.stem
    success, out = run_command(f"{TIPC} {flags} {file} -o {bc_file}")
    if not success:
        raise CompileError(out)
    success, out = run_command(f"{TIPCLANG} -w {bc_file} {RTLIB}/tip_rtlib.bc -o {executable_file}")
    if not success:
        raise CompileError(out)
    return str(executable_file)

def compare_files(expected_file: str, output_file: str, ignore_first_lines=0) -> str:
    """Compare the content of two files and return differences if any."""
    with open(expected_file) as expected, open(output_file) as output:
        diff = list(difflib.unified_diff(expected.readlines()[ignore_first_lines:], output.readlines()[ignore_first_lines:]))
        return diff

def compile_and_execute(file: Path, category: str, optimized = True, should_succeed = True):
    flags = "--do" if not optimized else ""
    with tempfile.TemporaryDirectory() as tmp:
        try:
            tmp = Path(tmp)
            executable = compile_file(file, tmp, flags=flags)
        except CompileError as e:
            success = False
            failure_msg = f"Failed to compile: {file}\n{e}"
        else:
            success, out = run_command(f"{executable}")
            optimized_str = "optimized" if optimized else "unoptimized"
            failure_msg = f"Failed {optimized_str} {category}: {file}\n{out}"
    success = success == should_succeed
    return success, "" if success else failure_msg

def check_file_suffix(file: Path, suffix: str):
    if file.suffix != suffix:
        raise RuntimeError(f"Expected a file extension of {suffix}, instead got: {file.suffix}")

def wrap_test_func(func, doc="", *args, **kwargs):
    @wraps(func)
    def new_execute(file: Path):
        return func(file, *args, **kwargs)
    new_execute.__doc__ = doc if doc else func.__doc__
    return new_execute

def self_test_with_category_name(category: str, optimized=True, should_succeed=True):
    optimized_str = "optimized" if optimized else "unoptimized"
    should_succeed_str = ", expected to fail" if not should_succeed else ""
    doc = f"self-contained, {optimized_str}{should_succeed_str}"
    return wrap_test_func(compile_and_execute, doc, category, optimized=optimized, should_succeed=should_succeed)

def diff_test_execute(expected_file, output_file, ignore_first_lines=0):
    diff = compare_files(expected_file, output_file, ignore_first_lines=ignore_first_lines)
    if diff:
        return False, f"Differences in test output:\n{''.join(diff)}"
    return True, ""

def test_compare_expected_output(file: Path):
    """program output"""
    check_file_suffix(file, ".expected")
    expected_file = file
    base, input_num = file.stem.split('-')
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        output_file = tmp / f"{base}.output"
        tip_file = file.parent / f"{base}.tip"
        executable = compile_file(tip_file, tmp)
        run_command(f"{executable} {input_num} > {output_file}")
        return diff_test_execute(expected_file, output_file)

def test_pretty_print_diff(file: Path, poly_inference=False):
    """pretty-print"""
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        output_file = tmp / f"{file.stem}.pppt.output"
        expected_file = f"{file}.pppt"
        pi_flag = "--pi" if poly_inference else ""
        run_command(f"{TIPC} --pp --pt {pi_flag} {file} > {output_file}", check=True)
        return diff_test_execute(expected_file, output_file)

def test_pretty_print_and_symbol_print(file: Path):
    """pretty-print and symbol-print"""
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        output_file = tmp / f"{file.stem}.ppps.output"
        run_command(f"{TIPC} --pp --ps {file.with_suffix('.tip')} > {output_file}", check=True)
        return diff_test_execute(file, output_file)

def test_ll_file_exists_after_asm(file: Path):
    """ll existence"""
    success = False
    try:
        expected_file = Path(f"{file}.ll")
        run_command(f"{TIPC} --asm {file}", check=True)
        success = expected_file.exists()
    finally:
        expected_file.unlink(missing_ok=True)
    return success, f"Did not find expected output, {expected_file}, for input {file}"

def test_ll_file_in_specified_location(file: Path):
    """specified ll existence"""
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        output_file = tmp / f"{file.stem}.tip.ll"
        expected_file = Path(f"{file}.ll")
        run_command(f"{TIPC} --asm {file} -o {output_file}", check=True)
        # the first 3 lines contain metadata we don't care about
        return diff_test_execute(expected_file, output_file, ignore_first_lines=3)

def test_output_graph(file: Path):
    """output graph"""
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        output_bc = tmp / f"{file.stem}.tip.bc"
        output_graph = tmp / f"{file.stem}.tip.dot"
        expected_graph = Path(f"{file}.dot")
        run_command(f"{TIPC} {file} --pcg={output_graph} -o {output_bc}", check=True)
        # the first 3 lines contain metadata we don't care about
        return diff_test_execute(expected_graph, output_graph)
    
def get_nonexistant_file() -> Path:
    file = Path("")
    while file.exists():
        file = Path(str(random.random()))
    return file

def test_nonexistant_file(file: Path):
    """nonexistant file"""
    nonexistant_file = get_nonexistant_file()
    success = False
    try:
        with tempfile.TemporaryDirectory() as tmp:
            compile_file(nonexistant_file, Path(tmp))
    except CompileError as e:
        success = True
    return success, f"Test failure for non-exisitent input: {nonexistant_file}"

def test_unwritable_output_file(file: Path, flag: str):
    """unwritable output"""
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        unwritable_file = tmp / "unwritable"
        unwritable_file.touch()
        unwritable_file.chmod(0o444)  # Make the file read-only
        command = f"{TIPC} {file} --{flag}={unwritable_file}"
        success, output = run_command(command)
        if success:
            return False, f"No error occurred when executing command: {command}"
        return "failed to open" in output, f"String 'failed to open' not found in command output:\n{output}"

def test_logging(file: Path):
    """logging"""
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        log_file = tmp / f"{file}.log"
        return run_command(f"{TIPC} --pt --log={log_file} {file}", check=True)
    
def test_ast_visualizer(file: Path):
    """AST Visualizer"""
    check_file_suffix(file, ".dot")
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        tip_file = file.with_suffix("")
        output_file = tmp / f"{file.stem}.output"
        run_command(f"{TIPC} --pa={output_file} {tip_file}", check=True)
        return diff_test_execute(file, output_file)


def collect_tests(filter_directories: list = None, filter_keyword: str = None):
    test_categories = [
        # (`directory`, `search_pattern`, `test_function`)
        # `test_function` accepts a `Path` as an argument and returns the tuple `(success: bool, output: str)`
        # In each of these tuples, for every `file` that matches `search_pattern` within `directory`,
            # we make sure that `test_function(file)` returns with `success==True`,
            # printing its `output` on a failure

        # Directory-wide tests
        ('selftests', '*.tip', self_test_with_category_name("selftest")),
        ('selftests', '*.tip', self_test_with_category_name("selftest", optimized=False)),
        ('selftests', '*.tip', test_pretty_print_diff),

        ('iotests', '*.expected', test_compare_expected_output),
        ('iotests', '*error.tip', self_test_with_category_name("errortest", should_succeed=False)),
        ('iotests', '*.ppps', test_pretty_print_and_symbol_print),

        ('polytests', "*.tip", wrap_test_func(test_pretty_print_diff, poly_inference=True)),

        # One-off tests
        ('iotests', 'main.tip', test_ll_file_exists_after_asm),
        ('iotests', 'fib.tip', test_ll_file_in_specified_location),
        ('iotests', 'fib.tip', test_output_graph),
        ('iotests', 'fib.tip', test_nonexistant_file),  # file 'fib.tip' doesn't matter
        ('iotests', 'linkedlist.tip', wrap_test_func(test_unwritable_output_file, flag="pa")),
        ('iotests', 'linkedlist.tip', wrap_test_func(test_unwritable_output_file, flag="pcg")),
        ('selftests', 'polyfactorial.tip', test_logging),
        ('iotests', 'linkedlist.tip.dot', test_ast_visualizer),
        ('selftests', 'ptr4.tip.dot', test_ast_visualizer),
    ]

    def filter_category(category):
        if filter_directories is None:
            return True
        directory, *_ = category
        return directory in filter_directories

    filtered_categories = filter(filter_category, test_categories)

    def filter_file(file: Path):
        if filter_keyword is None:
            return True
        return filter_keyword.lower() in file.name.lower()

    return [
        TestData(file, directory, execute)
        for directory, extension, execute in filtered_categories
        for file in filter(filter_file, Path(directory).glob(extension))
    ]

def run_all_tests(tests: list[TestData], serial: bool = False):
    """Run all tests, either serially or in parallel."""
    total_tests = len(tests)
    print(f"Running {total_tests} tests...")
    failures = []

    def execute_test(test: TestData):
        """Helper function to execute a single test."""
        passed = test.run()
        text, color = ("PASSED", Fore.GREEN) if passed else ("FAILED", Fore.RED)
        status = f"{color}{text}{Style.RESET_ALL}"
        print(f"{status}: {test}")
        if not passed:
            print(test.failure_output)
        return test if not passed else None

    if not serial:
        with concurrent.futures.ThreadPoolExecutor() as executor:
            future_to_test = {executor.submit(execute_test, test): test for test in tests}
            for future in concurrent.futures.as_completed(future_to_test):
                failure = future.result()
                if failure:
                    failures.append(failure)
    else:
        for test in tests:
            failure = execute_test(test)
            if failure:
                failures.append(failure)

    return failures

def print_summary(num_tests, failed_tests):
    num_failures = len(failed_tests)
    if num_failures == 0:
        print(f"\nAll {num_tests} tests passed")
    else:
        print(f"\n{num_failures}/{num_tests} tests failed")
        print("Failed tests:")
        for test in failed_tests:
            print(f"- {test}")

def setup():
    os.chdir(f"{ROOT_DIR}/test/system")

def cleanup():
    pass

def run_system_tests(filter_directory=None, filter_keyword=None, serial=False):
    try:
        setup()
        tests = collect_tests(filter_directories=filter_directory, filter_keyword=filter_keyword)
        failed_tests = run_all_tests(tests, serial=serial)
        print_summary(len(tests), failed_tests)
    finally:
        cleanup()

def parse_arguments():
    parser = argparse.ArgumentParser(description="Run system tests with optional filtering.")
    parser.add_argument(
        '--directories', '-d',
        nargs='+',
        help='List of directories to filter tests by (e.g. selftests, iotests)',
    )
    parser.add_argument(
        '--keyword', '-k',
        help='Filename substring to filter tests by (e.g. error, fib)',
    )
    parser.add_argument(
        '--serial', '-s',
        action='store_true',
        help='Run tests in serial',
    )
    return parser.parse_args()

def main():
    if not TIPCLANG:
        print("error: TIPCLANG env var must be set")
        exit(1)

    args = parse_arguments()
    run_system_tests(
        filter_directory=args.directories,
        filter_keyword=args.keyword,
        serial=args.serial,
    )

if __name__ == '__main__':
    main()