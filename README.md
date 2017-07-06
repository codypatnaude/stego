<h1>Stego</h1>

<h2>Getting Started</h2>
<strong>Command line params</strong>
<ul>
    <li><strong>-i : </strong> input image - image that the message will be encoded into</li>
    <li><strong>-f : </strong> input file - message to encode into the image (currently chr 2 terminated but will change soon)</li>
    <li><strong>-i : </strong> output file - file for output. encryption will output a bmp, decryption will output a txt file</li>
    <li><strong>-encrypt : </strong> tell stego you want to run encryption</li>
    <li><strong>-decrypt : </strong> tell stego you want to run decryption</li> 
</ul>
<p><strong>Compile:</strong> gcc stego.c -o stego</p>
<p><strong>Encrypt image:</strong> ./stego -o output.bmp -i input.bmp -f input.txt -encrypt</p>
<p><strong>Decrypt message:</strong> ./stego -o output.txt -i output.bmp -encrypt</p>
<p>If all worked properly output.txt will match input.txt</p>

<h1>Comming soon</h1>
<ul>
    <li>Probably changing "-encrypt" and "-decrypt" to "-encode" and "-decode" as it's more accurate</li>
    <li>Currently messages are terminated by a chr 2, obviously an issue if you want to encode a file (like another image). Going to change this.</li>
    <li>Adding public/private key encryption before encoding and decryption after decoding.</li>
</ul>

