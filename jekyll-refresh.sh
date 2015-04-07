cd jekyll
npm install
bower install
grunt build
cd ..

# Copy the custom assets folders.
mkdir cri/libraries/cri
rsync -avz --exclude '.git' jekyll/dist/ cri/libraries/cri
