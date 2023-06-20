<div id="top"></div>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h1 align="center">Blitzar</h1>

<p align="center">
<img
 alt="blitzar logo"
 width="200px"
 src="https://github.com/spaceandtimelabs/blitzar/blob/assets/logo.png"/>
</p>

  <a href="https://github.com/spaceandtimelabs/blitzar/actions/workflows/release.yml">
    <img alt="Build State" src="https://github.com/spaceandtimelabs/blitzar/actions/workflows/release.yml/badge.svg">
  </a>

  <a href="https://twitter.com/intent/follow?screen_name=spaceandtimedb">
    <img alt="Twitter" src="https://img.shields.io/twitter/follow/spaceandtimedb.svg?style=social&label=Follow">
  </a>

  <a href="http://discord.gg/SpaceandTimeDB">
    <img alt="Discord Server" src="https://img.shields.io/discord/953025874154893342?logo=discord">
  </a>

  <a href="https://github.com/spaceandtimelabs/blitzar/blob/main/LICENSE">
    <img alt="License" src="https://img.shields.io/badge/License-Apache_2.0-blue.svg">
    </a>
  </a>

  <a href="https://en.cppreference.com/w/cpp/20">
    <img alt="C++ Logo" src="https://img.shields.io/badge/C%2B%2B-20-blue?style=flat&logo=c%2B%2B">
    </a>
  </a>

  <a href="https://www.linux.org/">
    <img alt="OS" src="https://img.shields.io/badge/OS-Linux-blue?logo=linux">
    </a>
  </a>

  <a href="https://www.linux.org/">
    <img alt="CPU" src="https://img.shields.io/badge/CPU-x86-red">
    </a>
  </a>

  <a href="https://developer.nvidia.com/cuda-downloads">
    <img alt="CUDA" src="https://img.shields.io/badge/CUDA-12.1-green?style=flat&logo=nvidia">
    </a>
  </a>

  <p align="center">
    Space and Time C++ library for running cryptographic zero-knowledge proofs algorithms on the CPU and GPU.
    <br />
    <a href="https://github.com/spaceandtimelabs/blitzar/issues">Report Bug</a>
    |
    <a href="https://github.com/spaceandtimelabs/blitzar/issues">Request a Feature</a>
  </p>
</div>

## About The Project

#### Overview
C++ library for running cryptographic zero-knowledge proof algorithms on the CPU and GPU.  
> **Note**  
> This repo contains the C++ implementation along with cbindings and a Rust sys crate. If you are using Rust, use the crate from the companion [blitzar-rs](https://github.com/spaceandtimelabs/blitzar-rs) repo.

The library provides

* Functions for doing group operations on [Curve-25519](https://en.wikipedia.org/wiki/Curve25519) and  [Ristretto25519](https://ristretto.group/) elements.
* An implementation of [Pippenger's algorithm](https://cacr.uwaterloo.ca/techreports/2010/cacr2010-26.pdf) for computing multi-scalar multiplications (MSMs) and generalized Pedersen commitments.
* An implementation of [Inner Product Argument Protocol](https://eprint.iacr.org/2017/1066.pdf) for producing and verifying a compact proof of the inner product of two vectors.
* A sys-crate and bindings to make commitment computations usable from 
[Rust](https://github.com/spaceandtimelabs/blitzar-rs).

The library is adopted from code in [libsodium](https://github.com/jedisct1/libsodium) and extends libsodium's cryptographic functions to support CUDA so that they are usable on GPUs.

**WARNING**: This project has not undergone a security audit and is NOT ready
for production use.

#### Computational Backends
Although the primary goal of this library is to provide GPU acceleration for cryptographic zk proof algorithms, the library also provides CPU support for the sake of testing. So the following backends are supported:

| Backend            | Implementation                                             | Target Hardware             |
| :---               | :---                                                       | :---                        |
| `cpu`             | Serial      | x86 capable CPUs |
| `gpu`             | Parallel   | Nvidia CUDA capable GPUs


## Cryptographic Primitives

#### Multi-Scalar Multiplication (MSM) / Generalized Pedersen Commitment / Multiexponentiation

Blitzar provides an implementation of Multi-Scalar Multiplication (i.e. generalized Pedersen commitments)

Let $g_0\ldots g_n\in \mathbb{G}$ be elements of a group (with prime order), and let $a_0\ldots a_n\in\mathbb{F}$ be elements of the corresponding scalar field. (i.e. the field $\mathbb{F}_p$ where $p$ is the order of the group.)

Then, the Generalized Pedersen Commitment of the vector $\mathbf{a}=(a_1,\ldots, a_n)$ is
```math
P = a_1\cdot g_1+\cdots+ a_n\cdot g_n
```

Note: we interchangeably use the terms "multi-scalar multiplication" and "multiexponentiation" to refer to the this operation because when the group is written additively, the operation is a multi-scalar multiplication, and when the group is written multiplicatively, the operation is a multiexponentiation.

The Blitzar implementation allows for computation of multiple, potentially different length, MSMs simultaneously. Additionally, either built-in, precomputed, generators $g_n$ can be used, or they can be provided as needed.

Currently, Blitzar supports Curve25519 as the group. We will be expanding the curves that we support.

#### Inner Product Argument

Blitzar provides a modified implementation of an inner product argument (e.g. [Bulletproofs](https://eprint.iacr.org/2017/1066.pdf) and [Halo2](https://zcash.github.io/halo2/background/pc-ipa.html)).

Given generators $g_1, \ldots, g_n$; Pedersen commitment $P$; scalar $c$; and vectors 
$\mathbf{a}=(a_1,\ldots, a_n)$ and $\mathbf{b}=(b_1,\ldots, b_n)$;
Blitzar's version of the inner product proof allows a Prover to establish that
```math
\begin{aligned}
P &= a_1\cdot g_1+\cdots+ a_n\cdot g_n \\
c &= \langle \mathbf{a}, \mathbf{b} \rangle = a_1\cdot b_1+\cdots+ a_n\cdot b_n
\end{aligned}
```
where it is assumed that $\boldsymbol{g}$, $\boldsymbol{b}$, and $c$ are known to both the Prover
and Verifier.

This version of the inner product argument can be used in the context of a broader protocol.

#### Other Features to Come

If there is a particular feature that you would like to see, please [reach out](https://github.com/spaceandtimelabs/blitzar/issues). We are looking to broaden support.

## Performance ([associated commit hash](https://github.com/spaceandtimelabs/blitzar/commit/a03f80bae30546c0b02d7e968056e3473be3a851))

Benchmarks are run against four different types of GPU:

* [Nvidia 3060](https://www.nvidia.com/en-us/geforce/graphics-cards/30-series/rtx-3060-3060ti/)
* [Nvidia T4 - Standard_NC4as_T4_v3](https://learn.microsoft.com/en-us/azure/virtual-machines/nct4-v3-series)
* [Nvidia V100 - Standard_NC6s_v3](https://learn.microsoft.com/en-us/azure/virtual-machines/ncv3-series)
* [Nvidia A100 - Standard_NC24ads_A100_v4](https://learn.microsoft.com/en-us/azure/virtual-machines/nc-a100-v4-series)

<details open>
<summary>Multi-Scalar Multiplication / Generalized Pedersen Commitment Results:</summary>

The subsequent outcomes are derived from the preceding benchmark execution of the pedersen commitment, during which the number of sequences, bytes per element, sequence length, and GPU type were varied.

![Multiexponentiation Benchmarks](https://github.com/spaceandtimelabs/blitzar/blob/assets/benchmark/multiexponentiation_seconds.png)

</details>

<details open>
<summary>Inner Product Argument Results:</summary>

The subsequent outcomes are derived from the preceding benchmark execution of the inner product, during which the number of elements and the type of GPU were changed.

![Inner Product Benchmarks](https://github.com/spaceandtimelabs/blitzar/blob/assets/benchmark/innerprod_seconds.png)

</details>



## Getting Started

See the [example](./example) folder for some examples.

### Prerequisites to build from source

<details open>
<summary>GPU backend prerequisites:</summary>

* `x86_64` Linux instance.
* Docker installed (check [install guidelines](https://docs.docker.com/engine/install/ubuntu/)).
* Nvidia GPU capable of run CUDA 12.1 code.
* Recommended Nvidia Toolkit Driver Version: >= 530.30.02 (check the [compatibility list here](https://docs.nvidia.com/cuda/cuda-toolkit-release-notes/index.html)).
* [Nvidia container toolkit](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html) (necessary only if you intend to run tests and benchmarks on the GPU).

From your terminal, run the following command to start the GPU docker container:
```bash
./ci/docker/run_docker_gpu.sh
```
</details>

<details>
<summary>CPU backend prerequisites:</summary>

You'll need the following requirements to run the environment:

* `x86_64` Linux instance.
* Docker installed (check [install guidelines](https://docs.docker.com/engine/install/ubuntu/)).

From your terminal, run the following command to start the CPU docker container:
```bash
./ci/docker/run_docker_cpu.sh
```

</details>

<details>

<summary> General List of prerequisites: </summary>

Check the [prerequisites list](./ci/docker/Dockerfile) for the complete list of prerequisites.

</details>

## Usage

<details open>
<summary>Building and Testing the C++/CUDA code:</summary>

```bash
./ci/docker/run_docker_gpu.sh

# build all the code assets
bazel build //...

# run all tests
bazel test //...

# run all memory sanitized tests
bazel test --config=asan //...
```

Note: some tests will fail in case you don't have a GPU available.

</details>

<details open>
<summary>Building and Testing the Rust Sys-Crate code:</summary>

```bash
./ci/docker/run_docker_gpu.sh

# build the c++/cuda shared library and generate the rust bindings
./ci/build.sh 0.0.0

# run the sys-crate tests
cargo test --manifest-path rust/blitzar-sys/Cargo.toml
```

Although possible, this sys-crate is not meant to be used directly by Rust users. Instead, consider using the [blitzar-rs](https://github.com/spaceandtimelabs/blitzar-rs), which is a high-level wrapper around this sys-crate.

Note: the shared library byproduct of the C++/CUDA code is automatically copied to the Rust sys-crate under the `rust/blitzar-sys/` directory.

</details>

## Add to your project

You can find release ready versions of this library under the [release page](https://github.com/spaceandtimelabs/blitzar/releases/). You can also build and test the library from source by following the instructions below.

<details>

<summary>C++ Project:</summary>

See the C++ example here: [example/cbindings1/main.cc](./example/cbindings1/main.cc). To run this example, execute:

```bash
./ci/docker/run_docker_gpu.sh

bazel run //example/cbindings1:cbind1
```

Alternatively, compile this example code with g++:

```bash
./ci/docker/run_docker_gpu.sh

# Build the shared library
bazel build -c opt --config=portable_glibc //cbindings:libblitzar.so

# Compile the c++ example code
g++ example/cbindings1/main.cc -o main -I . -L bazel-bin/cbindings/ -lblitzar

# Execute the example code
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:bazel-bin/cbindings/ ./main
```
</details>

<details>

<summary>Rust Project:</summary>

See the Rust example here: [rust/tests/src/main.rs](./rust/tests/src/main.rs). To run this example, execute:

```bash
./ci/docker/run_docker_gpu.sh

./ci/build.sh 0.0.0

cargo test --manifest-path rust/tests/Cargo.toml
```

</details>

## Development Process

The main branch is regularly built and tested, being the only source of truth. [Tags](https://github.com/spaceandtimelabs/blitzar/tags) are created regularly from automated semantic release executions. 

#### Code format:

This repository follows some C++ formatting rules. To check if your modified code is compliant, run the following commands:

```bash
./ci/docker/run_docker_gpu.sh

./tools/code_format/check_format.py check

./tools/code_format/check_format.py fix
```

## Executing the Benchmarks:

<details open>
<summary>Generalized Pedersen Commitment (MSM):</summary>

```bash
./ci/docker/run_docker_gpu.sh

# Usage: benchmark <cpu|gpu> <n> <num_samples> <num_commitments> <element_nbytes> <verbose>
# - n: the size of the multiexponentiation vector (or the sequence length).
# - num_samples: the number of times to run the benchmark.
# - num_commitments: the number of commitments to generate.
# - element_nbytes: the number of bytes of each element in the vector (exponent size).
# - verbose: whether to print the results of each run.
bazel run -c opt //benchmark/multi_commitment:benchmark -- gpu 1000 5 1 32 1
```

</details>

<details open>
<summary>Inner Product Argument:</summary>

```bash
./ci/docker/run_docker_gpu.sh

# Usage: benchmark <cpu|gpu> <n> <num_samples>
# - n: the size of the inner product vector (or the sequence length).
# - num_samples: the number of times to run the benchmark.
bazel run -c opt //benchmark/inner_product_proof:benchmark -- gpu 1000 5
```

</details>


## Contributing

We are not accepting community Pull Requests yet due to logistic reasons. However, feel free to contribute with any suggestion, idea, or bugfix on our [Issues](https://github.com/spaceandtimelabs/blitzar/issues) panel. Also, see [contribution guide](CONTRIBUTING.md).

## Community & support

Join our [Discord server](https://discord.com/invite/SpaceandTimeDB) to ask questions, discuss features, and for general chat.

## License

This project is released under the [Apache 2 License](LICENSE).
  
## Rust crate

This repo contains the C++ implementation along with cbindings and a rust sys crate. If you are using rust, use the crate from the companion repo here: https://github.com/spaceandtimelabs/blitzar-rs.