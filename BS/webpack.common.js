'use strict';

const path = require('path');
const CleanWebpackPlugin = require('clean-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
  context: path.join(__dirname, "frontend"),
  entry: {
    app: './index.js',
  },

  resolve: {
    extensions: [
      ".js", ".yml"
    ],
    alias: {
      components: path.resolve(__dirname, "frontend/components"),
      store: path.resolve(__dirname, "frontend/store"),
    },
  },
  plugins: [
    new CleanWebpackPlugin(['dist']),
    new HtmlWebpackPlugin({
      title: ">>",
      template: './index.hbs',
      chunks: ['app'],
    }),
  ],
  output: {
    filename: 'bundle.js',
    path: path.join(__dirname, 'dist'),
  },
  module: {
    rules: [{
      test: /\.js$/,
      exclude: /node_modules/,
      use: [{
        loader: "babel-loader",
        options: {
          presets: ["es2015", "stage-0", "react"],
          plugins: ["transform-decorators-legacy", "transform-runtime"],
        }
      }]
    }, {
      test: /\.yml$/,
      exclude: /node_modules/,
      use: [{
        loader: "json-loader",
      }, {
        loader: "yaml-loader",
      }],
    }]
  }
};
