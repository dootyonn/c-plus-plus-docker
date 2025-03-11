# Use the official Ubuntu image as the base image
FROM ubuntu:24.04

# Set the working directory in the container
WORKDIR /app

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
  g++ \
  ninja-build \
  cmake \
  gdb \
  libgtest-dev \
  libcpprest-dev \
  libssl-dev


# Copy the source code into the container
COPY * .

# Compile the C++ code
RUN mkdir build
WORKDIR /app/build
RUN cmake .. && make

# Expose the port on which the API will listen
EXPOSE 8080

# Command to run the API when the container starts
CMD ["/app/build/okapi"]
