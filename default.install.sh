#!/bin/bash

# Modify the MySQL settings below so they will match your own.
MYSQL_USERNAME="root"
MYSQL_PASSWORD="root"
MYSQL_HOST="localhost"
MYSQL_DB_NAME="fb_safety"

# Modify the URL below to match your project's base domain URL.
BASE_DOMAIN_URL="http://localhost/fb_safety/www"

# Modify the login details below to be the desired login details for the Administrator account.
ADMIN_USERNAME="admin"
ADMIN_PASSWORD="admin"
ADMIN_EMAIL="admin@example.com"

rm -rf www/
mkdir www

bash scripts/build


# Build the jekyll to get the compiled CSS files.
cd jekyll
bundle install
npm install
bower install
grunt build
cd ..

# Copy the custom assets folders.
mkdir fb_safety/libraries/fb_safety
rsync -avz --exclude '.git' jekyll/dist/ fb_safety/libraries/fb_safety

cd www

drush si -y fb_safety --locale=en --account-name=$ADMIN_USERNAME --account-pass=$ADMIN_PASSWORD --account-mail=$ADMIN_EMAIL --db-url=mysql://$MYSQL_USERNAME:$MYSQL_PASSWORD@$MYSQL_HOST/$MYSQL_DB_NAME --uri=$BASE_DOMAIN_URL

chmod -R 777 sites/default/files

# Development modules
drush en devel views_ui field_ui migrate_ui -y

# Convert csv files to sql tables.
cd ..
bash csv2sql.sh
cd www

# These commands migrates dummy content and is used for development and testing. Comment out both lines if you so not wish to migrate dummy content, e.g on your live website.
drush mi --all

# This command does the login for you when the build script is done. It will open a new tab in your default browser and login to your project as the Administrator. Comment out this line if you do not want the login to happen automatically.
drush uli --uri=$BASE_DOMAIN_URL
