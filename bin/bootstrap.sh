#!/bin/sh
set -ex

ANTLR_VERSION=4
JAVA_VERSION=11
LLVM_VERSION=17

ROOT_DIR=${GITHUB_WORKSPACE:-$(git rev-parse --show-toplevel)}

bootstrap_ubuntu_dependencies() {
  [ -d /usr/share/keyrings ] || sudo mkdir -p /usr/share/keyrings

  wget https://apt.corretto.aws/corretto.key
  gpg --dearmor corretto.key
  rm corretto.key
  sudo mv corretto.key.gpg /usr/share/keyrings/amazon-corretto-${JAVA_VERSION}-keyring.gpg
  sudo cp "${ROOT_DIR}/bin/apt/amazon-corretto-${JAVA_VERSION}.sources" /etc/apt/sources.list.d/
  sudo apt -y update

  wget https://apt.kitware.com/kitware-archive.sh
  chmod +x kitware-archive.sh
  sudo ./kitware-archive.sh
  rm kitware-archive.sh

  sudo apt -y install \
    java-$JAVA_VERSION-amazon-corretto-jdk \
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
  rm llvm.sh

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
    python3-clang-$LLVM_VERSION \
    graphviz \
    jq \
    doxygen
}


bootstrap_ubuntu_env() {
  echo export LLVM_DIR="$(llvm-config-$LLVM_VERSION --prefix)/lib/cmake" >> ~/.bashrc
  echo export TIPCLANG="$(llvm-config-$LLVM_VERSION --bindir)/clang" >> ~/.bashrc
}


bootstrap_ubuntu() {
  bootstrap_ubuntu_dependencies
  bootstrap_ubuntu_env
}


bootstrap_linux() {
  . /etc/os-release
  if [ "$ID" = ubuntu ] || [ "$ID" = pop ]; then
    bootstrap_ubuntu
  else
    echo "$ID is not supported."
    exit 1
  fi
}


bootstrap_mac_env() {
  case $SHELL in
    */zsh)
      echo "export LLVM_DIR=$(brew --prefix llvm@$LLVM_VERSION)/lib/cmake" >> ~/.zshrc
      echo "export TIPCLANG=$(brew --prefix llvm@$LLVM_VERSION)/bin/clang" >> ~/.zshrc
      cat ~/.zshrc
      ;;
    */bash)
      # The macOS github runner does not include zsh.
      echo "export LLVM_DIR=$(brew --prefix llvm@$LLVM_VERSION)/lib/cmake" >> ~/.bashrc
      echo "export TIPCLANG=$(brew --prefix llvm@$LLVM_VERSION)/bin/clang" >> ~/.bashrc
      cat ~/.bashrc
      ;;
    *)
      echo "error: $SHELL is not supported."
  esac
}


bootstrap_mac_dependencies() {
  if ! [ -x "$(command -v brew)" ]; then
    echoerr error: Homebrew is not installed.
    exit 1
  fi

  brew install --cask corretto@$JAVA_VERSION
  brew install \
    git \
    cmake \
    pkg-config \
    llvm@$LLVM_VERSION \
    antlr \
    lcov \
    graphviz \
    jq \
    doxygen
}


bootstrap_mac() {
  bootstrap_mac_dependencies
  bootstrap_mac_env
}


bootstrap() {
  unameOut="$(uname -s)"
  case "${unameOut}" in
    Linux*)
      bootstrap_linux;;
    Darwin*)
      bootstrap_mac;;
    *)
      echoerr "error: Script has not been implemented for: ${unameOut}."
      exit 1
      ;;
  esac

  echo
  echo '---------------------------------------------------------------------------------'
  echo '* your rc has been updated - be sure to source the file or restart your shell.  *'
  echo '---------------------------------------------------------------------------------'
}

bootstrap
