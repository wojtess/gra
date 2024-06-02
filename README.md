Simple 2d game.

to clone use `git clone --recurse-submodules <repo_ulr>` or `git clone --recursive <repo_url>` or if you cloned first and forgot to clone with submodules use `git submodule update --init --recursive` in repo dir

to compiler use cmake and make, make will take some time beacuse raylib will be downloaded. <br />
`mkdir build` <br />
`cd build` <br />
`cmake ..`<-- this command will be slow depending on network bandwtch <br />
`make -j` <-- -j is for performacne, this means that all cores will be used <br />

project uses raylib and imgui <br />

[![wakatime](https://wakatime.com/badge/user/8a014d54-4ddb-41d1-9737-edd924afaae4/project/10af8499-4920-42a2-955b-9287c4b51c70.svg)](https://wakatime.com/badge/user/8a014d54-4ddb-41d1-9737-edd924afaae4/project/10af8499-4920-42a2-955b-9287c4b51c70)