const path = require('path');
const outputDir = path.join(__dirname, 'example/');

const isPrepack = process.env.NODE_ENV === 'prepack';

const makeConfig = () => {
  if (isPrepack) {
    return {
      entry: path.join(__dirname, 'lib/js/src/index.bs.js'),
      mode: 'production',
      output: {
        path: path.join(__dirname, 'bundled/'),
        filename: 'reactplastic.js',
      },
    };
  }
  return {
    entry: './example/index.js',
    mode: 'production',
    output: {
      path: outputDir,
      publicPath: outputDir,
      filename: 'example.js',
    },
    module: {
      rules: [
        {
          test: /\.m?js$/,
          exclude: /(node_modules|bower_components)/,
          use: {
            loader: 'babel-loader',
            options: {
              presets: ['@babel/preset-env', '@babel/preset-react'],
            },
          },
        },
      ],
    },
  };
};

module.exports = makeConfig();
