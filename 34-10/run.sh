executable_path="../build/34-10"
message_path="message"
out_path="../build/out"

if [ -d "$out_path" ]; then
    echo "Directory exists"
else
    echo "Directory does not exist. Creating now"
    mkdir -p $out_path
    echo "Directory created"
fi

executable_full=$(realpath $executable_path)
message_full=$(realpath $message_path)
out_full=$(realpath $out_path)

cd $executable_full

./genkey $out_full/priv
./conv $out_full/priv $out_full/publ
./sign $out_full/priv $message_full $out_full/sign
./verify $out_full/publ $message_full $out_full/sign
