# ELASTIC COLLISION IN OPENGL CPP

## Setup Instructions

### For WSL

1. First, download XLaunch from [SourceForge](https://sourceforge.net/projects/vcxsrv/) and set up the settings accordingly.
2. Run WSL and execute the following commands:

   ```sh
   export DISPLAY=$(grep nameserver /etc/resolv.conf | awk '{print $2}'):0.0
   export MESA_LOADER_DRIVER_OVERRIDE=zink
   ```

3. Add these lines to your `~/.bashrc` file and apply the changes:

   ```sh
   nano ~/.bashrc
   ```

   Paste the above lines at the end of the file and save it. Then run:

   ```sh
   source ~/.bashrc
   ```

4. Clone the repository:

   ```sh
   git clone https://github.com/kausik10/elastic_collision_in_OpenGL_cpp.git
   ```

5. Navigate to the project directory:

   ```sh
   cd elastic_collision_in_OpenGL_cpp
   ```

6. Run the `make` command to build the project:

   ```sh
   make
   ```

### For Linux

1. Install the necessary dependencies:

   ```sh
   sudo apt update
   sudo apt install build-essential mesa-utils libgl1-mesa-dev libglew-dev libglfw3-dev
   ```

2. Clone the repository:

   ```sh
   git clone https://github.com/kausik10/elastic_collision_in_OpenGL_cpp.git
   ```

3. Navigate to the project directory:

   ```sh
   cd elastic_collision_in_OpenGL_cpp
   ```

4. Run the `make` command to build the project:

   ```sh
   make
   ```

