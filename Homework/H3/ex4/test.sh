key=$(echo "remi" | md5sum | head -c 32)
echo "Key: $key"
pt=$(xxd -p hoge | tr -d '\n')
echo "Plain hex: $pt"
openssl aes-128-ecb -K $key -nosalt -in hoge -out enc
ct=$(xxd -p enc | tr -d '\n')
echo "Cipher hex: $ct"
dec=$(openssl aes-128-ecb -d -K $key -nosalt -in enc)
echo "Decrypted hex: $dec"
