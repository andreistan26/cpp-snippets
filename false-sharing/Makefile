TARGET=false-sharing

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	g++ -O3 $< -o $@

perf-c2c:
	perf c2c record ./$(TARGET)

perf-c2c-report:
	perf c2c report --stdio
