var gulp = require('gulp');
var autoprefixer = require('gulp-autoprefixer');
var minifyHtml = require('gulp-minify-html');
var imagemin = require('gulp-imagemin');
var minifyCss = require('gulp-minify-css');
var sass = require('gulp-sass');
var uglify = require('gulp-uglify');

gulp.task('images', function() {
    gulp.src('favicon.ico')
        .pipe(gulp.dest('./output/www/')
    );
    gulp.src('favicon.png')
        .pipe(imagemin({
            optimizationLevel: 7
        })).on('error', errorHandler)
        .pipe(gulp.dest('./output/www/')
    );
    gulp.src('images/**/*.{gif,jpg,png,svg}')
        .pipe(imagemin({
            multipass: true,
            optimizationLevel: 7,
            progressive: true,
        })).on('error', errorHandler)
        .pipe(gulp.dest('./output/www/img/')
    );
});

gulp.task('scripts', function() {
    gulp.src(['javascripts/**/*.js', '!javascripts/**/*.min.js'])
        .pipe(uglify()).on('error', errorHandler)
        .pipe(gulp.dest('./output/www/js/')
    );
    gulp.src(['javascripts/**/*.min.js'])
        .pipe(gulp.dest('./output/www/js/')
    );
});

gulp.task('styles', function() {
    gulp.src('stylesheets/**/*.scss')
        .pipe(sass()).on('error', errorHandler)
        .pipe(autoprefixer('last 2 versions')).on('error', errorHandler)
        .pipe(gulp.dest('./output/css-temp/'))
        .pipe(minifyCss()).on('error', errorHandler)
        .pipe(gulp.dest('./output/www/css/')
    );
});

gulp.task('templates', function() {
    gulp.src('templates/**/*.wtml')
        .pipe(minifyHtml()).on('error', errorHandler)
        .pipe(gulp.dest('./output/templates/')
    );
});

gulp.task('wt-resources', function() {
    // Images
    gulp.src('resources/**/*.{gif,jpg,png,svg}')
        .pipe(imagemin({
            multipass: true,
            optimizationLevel: 7,
            progressive: true,
        })).on('error', errorHandler)
        .pipe(gulp.dest('./output/www/resources/')
    );

    // Scripts
    gulp.src(['resources/**/*.js', '!resources/**/*.min.js'])
        .pipe(uglify()).on('error', errorHandler)
        .pipe(gulp.dest('./output/www/resources/')
    );

    // Styles
    gulp.src(['resources/**/*.css', '!resources/**/*.min.css'])
        .pipe(minifyCss()).on('error', errorHandler)
        .pipe(gulp.dest('./output/www/resources/')
    );

    // The rest of the files without overwriting the optimized ones
    gulp.src(['resources/**/*',
              '!resources/**/*.{gif,jpg,png,svg}',
              '!resources/**/*.js',
              '!resources/**/*.css'])
        .pipe(gulp.dest('./output/www/resources/')
    );
    gulp.src(['resources/**/*.min.css', 'resources/**/*.min.js'])
        .pipe(gulp.dest('./output/www/resources/')
    );
});

gulp.task('default', ['templates', 'scripts', 'styles', 'images', 'wt-resources']);

// Public error handler
function errorHandler (error) {
  console.log(error.toString());
  this.emit('end');
}


