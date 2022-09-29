OUTPUT_DIR=../
SOURCE_IDR=./
PROTOC_BIN=../../third_party/linux-x86_64/protobuf-3.20/bin/protoc
${PROTOC_BIN} --cpp_out=${OUTPUT_DIR} ${SOURCE_IDR}/login.proto
${PROTOC_BIN} --cpp_out=${OUTPUT_DIR} ${SOURCE_IDR}/keepalive.proto
${PROTOC_BIN} --cpp_out=${OUTPUT_DIR} ${SOURCE_IDR}/raw.proto
${PROTOC_BIN} --cpp_out=${OUTPUT_DIR} ${SOURCE_IDR}/user_info.proto
