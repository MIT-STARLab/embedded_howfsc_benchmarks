#!/bin/bash

# Constants
TARGET_IP="169.254.198.2"
REMOTE_PATH="~/benchmark/"
LIBRARY_FILE="/opt/fsl-qoriq/2.0/sysroots/ppce5500-fsl-linux/usr/lib/libgomp.so.1"
REMOTE_LIB_PATH="/usr/lib/"

# Check if the user has provided the target name
if [ -z "$1" ]; then
  echo "Usage: $0 <target_name>"
  echo "Example: $0 T1040"
  exit 1
fi

# Get target name from user input
TARGET_NAME=$1

# Set the binary file name based on the target name
BINARY_FILE="${TARGET_NAME}_benchmark"

# Paths
LOCAL_BINARY_FILE="./bin/$BINARY_FILE"

# Check if the binary file exists
if [ ! -f "$LOCAL_BINARY_FILE" ]; then
  echo "Error: Binary file $LOCAL_BINARY_FILE not found!"
  exit 1
fi

# Copy the binary file to the remote target
echo "Copying  $LOCAL_BINARY_FILE to $TARGET_IP:$REMOTE_PATH"

#scp -r "$LOCAL_DATA_FILES" root@$TARGET_IP:"$REMOTE_PATH"
scp "$LOCAL_BINARY_FILE" root@$TARGET_IP:"$REMOTE_PATH"

if [ $? -eq 0 ]; then
  echo "Files successfully copied to $TARGET_IP:$REMOTE_PATH"
else
  echo "Error during file transfer."
fi


# Copy the libgomp.so.1 library file to the remote target's library path
echo "Copying $LIBRARY_FILE to $TARGET_IP:$REMOTE_LIB_PATH"
scp "$LIBRARY_FILE" root@$TARGET_IP:"$REMOTE_LIB_PATH"

# Check if the library file transfer was successful
if [ $? -eq 0 ]; then
  echo "Library file successfully copied to $TARGET_IP:$REMOTE_LIB_PATH"
else
  echo "Error during library file transfer."
  exit 1
fi