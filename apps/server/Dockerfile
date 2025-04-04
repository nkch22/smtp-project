FROM ubuntu:latest AS builder

# Install remaining dependencies
RUN apt update && apt install -y \
    clang cmake make git \
    libpq-dev \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code and build
COPY . .
RUN mkdir build && cd build && cmake .. && make -j$(nproc)
#
FROM ubuntu:latest AS runtime

# Copy certificates into the runtime container
COPY tools /etc/smtp_server/tools
COPY configs /etc/smtp_server/configs

# Copy logger lib into the runtime container
COPY --from=builder /app/build/src/Logger/liblogger_shared.so /usr/lib/
COPY --from=builder /usr/lib/x86_64-linux-gnu/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/

# Verify that the shared library is available
RUN ldconfig  # Refresh dynamic linker cache

# Install runtime dependencies
RUN apt update && apt install -y libssl-dev && rm -rf /var/lib/apt/lists/*

# Copy the built server binary from the builder stage
COPY --from=builder /app/build/smtp_server /etc/smtp_server/

# Expose necessary ports (adjust as needed)
EXPOSE 465

WORKDIR /etc/smtp_server/

# Run the server
ENTRYPOINT ["./smtp_server"]