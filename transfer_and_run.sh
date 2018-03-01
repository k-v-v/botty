#!/bin/bash
scp -i /home/dimitar/.ssh/AWS.pem /home/dimitar/Documents/botty/cmake-build-release/botty ec2-user@35.176.239.85:/home/ec2-user/run
ssh -i /home/dimitar/.ssh/AWS.pem ec2-user@35.176.239.85 /home/ec2-user/run/botty