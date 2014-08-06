mysql-fifo-loader
=================

c++ myql fifo bulk loader class

Requirements
===
* mysql cli client located in $PATH

How it works
===
* create and open temp fifo file
* issue mysql cli command to run 'LOAD DATA' using fifo file
* write to fifo file handler
* destructor handles cleanup which will cause mysql commit

Inspired by Justin Swanhart - shard-loader.php
