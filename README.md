# DELINEATE

Delineate detects horizontal and vertical lines in an image. It allows you to identify continuous stretches of black lines as well as dashed and dotted lines that appear in an image.

Delineate is a feature detection library that is written in C and packaged in Ruby with the core of the library written as a C-extension to Ruby.

## Installation

Delineate is dependent on _OpenCV_ v2.4.3 and Ruby v1.9.3+. It is compatible with any *nix environment, so unfortunately Windows is excluded. In order to install Delineate, presuming you've installed Ruby, you'll need to install OpenCV. You can do this as follows:

    # On OSX, if you have homebrew (you may need to use sudo)
    brew install opencv

    # On Ubuntu
    sudo apt-get install opencv

Once opencv is installed, you can install Delineate by adding it to your Gemfile:

    gem 'delineate', :git => 'git@github.com:timols/delineate.git'

Or you can install it directly from the command line as follows:

    git clone git@github.com:timols/delineate.git
    cd delineate
    gem build delineate.gemspec
    gem install delineate-0.1.0.gem

## Usage

Delineate is designed to be used for detecting lines in an image (primarily text based documents converted to an image format). To identify horizontal lines on an image, it can be used as follows:

    Delineate.open_image("/path/to/image/file/to/open.png")
    #=> true

    min_length, skip_gap = 30, 1
    horizontal_lines = Delineate.horizontal_lines(min_length, skip_gap)
    #=> [<Delineate::Line width=(4.0) start_point=[1245, 3060] end_point=[1305, 3060]>, ...]

    Delineate.close_image
    #=> true

    first_line = horizontal_lines.first
    #=> <Delineate::Line width=(4.0) start_point=[1245, 3060] end_point=[1305, 3060]>

    first_line.orientation
    #=> :horizontal

Delineate#open_image needs to be called each time you wish to work on a new image. Delineate#close_image should be called when all feature detection for the image has been completed.

## Copyright

Copyright (c) 2014 Tim Olshansky. See LICENSE.txt for further details.
