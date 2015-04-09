cd jekyll
npm install
bower install
grunt build
cd ..

# Copy the custom assets folders.
mkdir fb-safety/libraries/fb-safety
rsync -avz --exclude '.git' jekyll/dist/ cri/libraries/cri
