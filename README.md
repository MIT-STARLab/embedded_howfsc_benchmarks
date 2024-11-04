# P5040_test_software

Contains code for the evaluation of the P5040 processor for the MIT APRA project.

## Top level folder contents

### math_utils

Math routines from Numerical Recipes in c, with some modification.

### EFC

Code for optical modeling with processor in the loop. The code in this folder includes both the c code that runs on the embedded processor and the python code that runs on the host. It does not include physical optical modeling. Currently we implement EFC without pairwise probing and without Jacobian recomputation.

### Benchmarking

Scripts to test the speed of the P5040 on fundamental math operations. This code is not used in the processor in the loop testing.


### Workflow

On the brandon/full_HITL branch, the .gitignore file ensures that test/, EFC/data_files/, EFC/python_host/__pycache__/, and EFC/python_host/*.pyc directories are not tracked. All files remain in the working directory in those directories on Homer, but they are not tracked anymore, meaning any changes inside the directories will not be tracked as possible files to stage and commit.

The issue is that the commit history still includes massive files that were commited, so we need to leverage another tool to clear the large files from the commit history, like  ```bfg --strip-blobs-bigger-than 100M```

This Stack Overflow post should help: https://stackoverflow.com/questions/73673092/how-to-use-bfg-repo-cleaner# embedded_howfsc
# embedded_howfsc_benchmarks
