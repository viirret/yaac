<div align="center">
<h1>yaac</h1>
<p> Yet another alarm clock </p>
</div>

## Introduction

I don't trust having just alarm in my phone to wake up. Sometimes my phone is on silent or I've pressed a button in my sleep. Few years ago I found a solution to my problem. I used a website on my laptop called [kukuklok.com](https://kukuklok.com/).

I loved this website, however I thought it could be improved.

This project aims to create kukuklok like experience but with:
- Custom mp3s
- Preconfigured wakeup time
- Custom colorscheme
- Less resources and internet being used

## Installing

If you're using Arch Linux, check out [holvi](https://github.com/NeuronActivation/holvi) to install with pacman
```
pacman -S yaac
```

### Manual installation
Install dependencies, for Ubuntu/Debian
```
apt install libsdl2-dev libsdl2-mixer-dev libsdl2-ttf-dev fonts-freefont-ttf deeping-sound-theme build-essential cmake
```

Clone the repository
```
git clone https://github.com/viirret/yaac.git
```
Build the executable
```
mkdir build && cd build
cmake ..
make
```
Run the executable
```
./yaac
```

## Configuration
This program can be configured with custom sound files and other settings.

To add custom sound files create directory for sound files
```
mkdir -p ~/.local/share/yaac
```
Add your file to there
```
cp /path/to/wakeupsong.mp3 ~./local/share/yaac
```

The configuration file is read from **~/.config/yaac/config**. [Example config](config)



