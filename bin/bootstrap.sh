#!/usr/bin/env bash
set -ex

declare -r ANTLR_VERSION=4
declare -r JAVA_VERSION=8
declare -r LLVM_VERSION=11

declare -r ROOT_DIR=${GITHUB_WORKSPACE:-$(git rev-parse --show-toplevel)}

echogreen() {
  local green=$(tput setaf 2)
  local reset=$(tput sgr0)
  echo "${green}${@}${reset}"
}


echoerr() {
  local red=$(tput setaf 1)
  local reset=$(tput sgr0)
  echo "${red}${@}${reset}" 1>&2;
  echo "$@" 1>&2;
}


bootstrap_ubuntu_dependencies() {
  [ -d /usr/share/keyrings ] || sudo mkdir -p /usr/share/keyrings

  wget https://apt.corretto.aws/corretto.key 
  gpg --dearmor corretto.key
  sudo mv corretto.key.gpg /usr/share/keyrings/amazon-corretto-8-keyring.gpg
  sudo cp ${ROOT_DIR}/bin/apt/amazon-corretto-8.sources /etc/apt/sources.list.d/
  sudo apt -y update

  wget https://apt.kitware.com/kitware-archive.sh
  chmod +x kitware-archive.sh
  sudo ./kitware-archive.sh

  sudo apt -y install \
    java-1.$JAVA_VERSION.0-amazon-corretto-jdk \
    git \
    cmake \
    pkg-config \
    uuid-dev \
    antlr$ANTLR_VERSION \
    zlib1g-dev \
    lcov
  
  wget https://apt.llvm.org/llvm.sh
  sed -i -E 's,Ubuntu_(.*),&\n    Pop_\1,g' llvm.sh
  chmod +x llvm.sh
  sudo ./llvm.sh $LLVM_VERSION

  sudo apt -y install \
    libllvm-$LLVM_VERSION-ocaml-dev \
    libllvm$LLVM_VERSION \
    llvm-$LLVM_VERSION \
    llvm-$LLVM_VERSION-dev \
    llvm-$LLVM_VERSION-doc \
    llvm-$LLVM_VERSION-examples \
    llvm-$LLVM_VERSION-runtime

  sudo apt -y install \
    clang-tools-$LLVM_VERSION \
    clang-$LLVM_VERSION-doc \
    libclang-common-$LLVM_VERSION-dev \
    libclang-$LLVM_VERSION-dev \
    libclang1-$LLVM_VERSION \
    clang-format-$LLVM_VERSION \
    python3-clang-$LLVM_VERSION
}


bootstrap_ubuntu_env() {
  echo export TIPCLANG=$(which clang-$LLVM_VERSION) >> ~/.bashrc
}


bootstrap_ubuntu() {
  bootstrap_ubuntu_dependencies
  bootstrap_ubuntu_env
}


bootstrap_linux() {
  . /etc/os-release
  if [ $ID == ubuntu -o $ID == pop ]; then
    bootstrap_ubuntu
  else
    echoerr $ID is not supported.
    exit 1
  fi
}


bootstrap_mac_env() {
  echo export LLVM_DIR=$(brew --prefix llvm@$LLVM_VERSION)/lib/cmake >> ~/.bashrc
  echo export TIPCLANG=$(brew --prefix llvm@$LLVM_VERSION)/bin/clang >> ~/.bashrc
}


bootstrap_mac_dependencies() {
  if ! [ -x "$(command -v brew)" ]; then
    echoerr error: Homebrew is not installed.
    exit 1
  fi

  brew install homebrew/cask-versions/corretto$JAVA_VERSION
  brew install \
    git \
    cmake \
    pkg-config \
    llvm@$LLVM_VERSION \
    antlr@$ANTLR_VERSION \
    lcov
}


bootstrap_mac() {
  bootstrap_mac_dependencies
  bootstrap_mac_env
}


bootstrap() {
  local unameOut="$(uname -s)"
  case "${unameOut}" in
    Linux*)
      bootstrap_linux;;
    Darwin*)
      bootstrap_mac;;
    *)
      echoerr error: Script has not been implemented for: ${unameOut}.
      exit 1
      ;;
  esac

  echogreen
  echogreen '--------------------------------------------------------------------------------'
  echogreen '* bashrc has been updated - be sure to source the file or restart your shell.  *'
  echogreen '--------------------------------------------------------------------------------'
}

bootstrap

