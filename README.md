Simple 2d game.

to clone use `git clone --recurse-submodules <repo_ulr>` or `git clone --recursive <repo_url>` or if you cloned first and forgot to clone with submodules use `git submodule update --init --recursive` in repo dir

to compiler use cmake and make, make will take some time beacuse raylib will be downloaded.
`mkdir build`
`cd build`
`cmake ..`
`make -j` <-- this command will be slow depending on network bandwtch, -j is for performacne, this means that all cores will be used

project uses raylib and imgui