import {Router as ExpressRouter, static as ExpressStatic} from 'express';
import API from 'Router/API';
import history from 'connect-history-api-fallback';
import path from 'path';

class Router extends ExpressRouter {
  root = path.join(__dirname, '../dist');

  constructor() {
    super();
    this.use('/api', API);
    this.use(history({
      verbose: true
    }));
    this.use(ExpressStatic(this.root));

    this.get('/', Router.root);
    this.use(Router.notFound);
    this.use(Router.errorHandler);
  }

  static notFound(req, res, next) {
    let error = new Error('Not Found');
    error.status = 404;
    next(error);
  }

  static errorHandler(err, req, res, next) {
    res.status(err.status || 500);
    res.json({
      'errors': {
        message: err.message,
        error: {}
      }
    });
    next();
  }

  static root(req, res) {
    res.sendFile(path.join(__dirname, 'index.html'));
  }
}

export default new Router();