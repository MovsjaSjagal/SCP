# Guide to Running C Code on a Linux Virtual Machine using CloudVeneto

This is a guide to creating, accessing, and using a Linux Virtual Machine on CloudVeneto. The guide explains, in particular, how to use the virtual environment to compile and run C/C++ programs. 

The guide assumes that the reader has access to CloudVeneto and is registered with a project. If this is not yet the case, or if the reader finds any trouble in attempting to do so, please consult the following link: https://userguide.cloudveneto.it/en/latest/Registration.html. 

## 1 Creating a Key Pair
The first task is to create a key pair that we shall use to access the virtual machine from our local environment. This key pair can be used for multiple instances. If the reader already possesses a key pair they can skip the remainder of this chapter.

We navigate to https://cloudveneto.ict.unipd.it/dashboard/project/, and on the left side of the website one should see a tab called "Compute", in which one finds a subtab called "Overview". Select it, then click on "Key Pairs". In the "Key Pairs" section, select "Create Key Pair". The pair should be downloaded immediately to your computer as a `.pem` file. 
It is important to save this key in a proper location, since it can be downloaded only once. It is conventional to save the key in your `.ssh` 
directory:

- **macOS/Linux:** `/home/your_username/.ssh`
- **Windows:** `C:\Users\your_username\.ssh\`

Now that the key is stored correctly, restrict its permissions so that SSH 
will accept it. 

**macOS/Linux:**
```bash
cd ~/.ssh
chmod 600 my_key.pem
```

**Windows (PowerShell):**
```powershell
$keyPath = "$env:USERPROFILE\.ssh\my_key.pem"
icacls $keyPath /inheritance:r /grant:r "$($env:USERNAME):R"
```

## 2 Launching an Instance via CloudVeneto
Now we are ready to set up the virtual machine.

Again within the "Compute" tab, find the subtab called "Instances". One should then see a list of all instances running within the project. Click on the button saying "Launch Instance", and a pop-up tab should be visible. 

- On this first page enter your preferred name and the amount of VMs you would like to launch (under "Count"), the standard being 1.   
- Click on "Next>", and select a source image, ideally the most recent Almalinux version, e.g. "AlmaLinux10-INFNPadova-2026-02-17".  
- Again click on "Next>", and select a flavor with the required sizing for the compute, memory and storage capacity of the instance. A standard choice is "cloudveneto.large".   
- Then, in the next tab, add an available network (typically <ProjectName>-lan).
- Move to the "Security groups" tab, and set default security.   
- Then add the key pair that you created before, and launch the instance. Your instance should now be visible under the project's subtab called "Instances". 

In summary, configure the instance with the following parameters:

    Instance Name: VM-Linux
    Image: AlmaLinux10-INFNPadova-2026-02-17
    Flavor: cloudveneto.large
    Key Pair: select your key
    Network: <ProjectName>-lan



## 3 Accessing a Linux VM from your Terminal

To access your virtual machine, we use **SSH (Secure Shell)**, which allows you to securely control the remote Linux system from your local terminal.

### A. Connecting via the Gate Server (if outside university network)

If you are not connected to the university network, you must first connect to the CloudVeneto gate server. This acts as an entry point to the internal network.

Run the following command in your local terminal:

```bash
ssh username@gate.cloudveneto.it
```

Replace `username` with your CloudVeneto username. After logging in, you are now inside the CloudVeneto network.

After a successful login, your prompt should start with:

```bash
bash-5.1$
```

---

### B. Uploading Your Private Key

Next, you need to make your private key available on the gate server so you 
can access your VM. Open a new local terminal (do not close your gate session) 
and run the appropriate command for your OS:

**macOS/Linux:**
```bash
scp ~/.ssh/my_key.pem username@gate.cloudveneto.it:~
```

**Windows (PowerShell):**
```powershell
scp $env:USERPROFILE\.ssh\my_key.pem username@gate.cloudveneto.it:~
```

This copies your key to the home directory of the gate server.

Now, on the gate server, organize and secure the key again:

```bash
mkdir -p ~/.ssh
mv ~/my_key.pem ~/.ssh/
chmod 600 ~/.ssh/my_key.pem
```

The permission command is important: SSH will refuse to use the key if it is publicly accessible.

---

### C. Connecting to Your Virtual Machine

You can now connect to your virtual machine using its **private IP address**, which can be found in the CloudVeneto "Instances" tab.

For AlmaLinux systems:

```bash
ssh -i ~/.ssh/my_key.pem almalinux@VM_IP
```

For Ubuntu systems:

```bash
ssh -i ~/.ssh/my_key.pem ubuntu@VM_IP
```

Replace `VM_IP` with your instance’s private IP address. If all goes well then your prompt will update to something like:

```bash
[almalinux@your-instance-name ~]$
```

This confirms that you are now connected to your virtual machine. You can now execute Linux commands and proceed with compiling and running your C/C++ programs.


## 4 Writing, Compiling, and Running C/C++ Code

### A. Installing the Required Packages

Before compiling C or C++ programs, you need to make sure that the necessary tools are installed on your virtual machine. Run the following commands:

```bash
sudo dnf install nano
sudo dnf install gcc gcc-c++
```

`nano` is a simple terminal text editor you will use to write your code. `gcc` and `gcc-c++` are the compilers for C and C++ respectively.

If a package is already installed, you will see a message like:

```
Package nano-8.1-3.el10.x86_64 is already installed.
Nothing to do.
Complete!
```

In that case, no action is needed and you can move on.

---

### B. Creating and Editing a Source File

To create a new C source file, use the `nano` command followed by your desired filename. For example:

```bash
nano solve_strong_CP.c
```

If a file with that name already exists in the current directory, it will be opened for editing rather than created fresh. For a C++ file, use the `.cpp` extension instead:

```bash
nano solve_strong_CP.cpp
```

Write your code in the editor. When finished, save and exit by pressing `Ctrl+O` (to save), then `Enter` to confirm, then `Ctrl+X` to exit.

---

### C. Compiling Your Code

Once your source file is saved, you can compile it. For a C file:

```bash
gcc solve_strong_CP.c -o solve_strong_CP
```

This tells `gcc` to compile `solve_strong_CP.c` and produce an executable named `solve_strong_CP`. For a C++ file, use `g++` instead:

```bash
g++ solve_strong_CP.cpp -o solve_strong_CP
```

If your code contains errors, the compiler will print them to the terminal with line numbers. Fix the errors in `nano`, save the file, and recompile before attempting to run the program.

---

### D. Running Your Program

Once compiled successfully, run your program with:

```bash
./solve_strong_CP
```

The `./` prefix tells the shell to look for the executable in the current directory. If your program produces output, it will be printed directly to the terminal.
