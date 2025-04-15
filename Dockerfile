FROM ubuntu:24.10

WORKDIR /usr/local/smtp-server

COPY . .

RUN apt update && \
    apt-get install -y cmake make python3 python3-pip pipx git pkg-config libssl-dev

RUN pip3 install --break-system-packages conan

RUN conan profile detect --force && \
    conan install . -s compiler.cppstd=17 --output-folder=build --build=missing

RUN cd build && \
    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release && \
    cmake --build .

EXPOSE 465
CMD ["./build/smtp_server", "config", "tools/certificates"]