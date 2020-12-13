sudo apt install make gcc -y

sudo apt install curl libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev libffi6 libffi-dev -y
wget https://www.python.org/ftp/python/3.7.6/Python-3.7.6.tgz
tar xvfz Python-3.7.6.tgz
sudo rm Python-3.7.6.tgz
cd Python-3.7.6
./configure LDFLAGES='-L/opt/local/lib -R/opt/local/lib'
sudo make
sudo make install
cd ..
sudo rm -rf Python-3.7.6

curl https://bootstrap.pypa.io/get-pip.py | sudo python3 --disable-pip-version-check --no-cache-dir
sudo echo "alias pip3='pip3 --trusted-host pypi.org --trusted-host files.pythonhosted.org'" >> ~/.bashrc
source ~/.bashrc
sudo pip3 install --upgrade pip

python3 --version
pip3 --version

echo
echo ---------dependencies done---------
echo
