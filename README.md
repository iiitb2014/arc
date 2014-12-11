Arc - Anonymous Random Chat
===========================

This chat program creates a group chat session where people chat anonymously. A log of the conversations will be maintained and is accessible to the user via the link chat. The entire chat application is accessible over the web. No user login is required.

Since the entire server site code is in C, with several optimizations, it can be scaled to any level. So with minimal modifications to the code this chat program can be used for large scale production environment. 

## To Get Arc

1. Install git: `sudo apt-get install git ssh`
2. Setup your git identity: 
```
git config --global user.email '<your email>'
git config --global user.name '<your name>'
```
3. [Set up your ssh](https://help.github.com/articles/generating-ssh-keys/)
4. Run: `git clone git@github.com:arkokoley/arc.git`
5. `cd arc`
6. Compile the server: `make`

## To Contribute

1. Make changes in the repository.
2. Add any new files to git index using: `git add <file names>`
3. Commit the changes: `git commit -am '<any message related to the change done>'`
4. Push to GitHub: `git push origin master`

## To Test

1. Place a html or any file in the arc folder.
2. Run: `./server -p 4000`
3. Open your browser and open the url: `localhost:4000/<any file placed in arc folder>`
