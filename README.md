Arc - Anonymous Random Chat
===========================

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
6. Compile the server: `gcc n.c -o n`

## To Contribute

1. Make changes in the repository.
2. Add any new files to git index using: `git add <file names>`
3. Commit the changes: `git commit -am '<any message related to the change done>'`
4. Push to GitHub: `git push origin master`

## To Test

1. Place a html or any file in the arc folder.
2. Run: `./n -p 4000`
3. Open your browser and open the url: `localhost:4000/<any file placed in arc folder>`
