# Generate key pair for SFTP communication
__NOTE__: Keep the passpharse and private key secret. The folder ./files/config-files/ has been ignored by git.  
_________________
Generate an private/public key pair for the SFTP communication  
```console
[pi@raspberry:~]$ sudo ssh-keygen -m PEM -t rsa -N "" -f ./files/config-files/ssh-key
```
Upload the public key to the server so that it will trust us for accessing the specified user's account
```console
[pi@raspberry:~]$ sudo ssh-copy-id -i ./files/config-files/ssh-key.pub <server_username>@<server_IP>
```
Save the server as known host
```console
[pi@raspberry:~]$ ssh-keyscan <server_IP> > ./files/config-files/known-hosts
```
If any path change, save the configurations to the configuration file
```console
[pi@raspberry:~]$ nano ./files/config.txt