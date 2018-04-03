import {Router as ExpressRouter} from 'express';
import Users from 'Router/API/Users';

class API extends ExpressRouter {
  constructor() {
    super();
    this.use(Users);
    this.use(API.handleValidationError);
  }

  static handleValidationError(err, req, res, next) {
    if (err.name === 'ValidationError') {
      return res.status(422).json({
        error: Object.keys(err.errors).reduce((errors, key) => {
          errors[key] = err.errors[key].message;
          return errors;
        }, {}),
      });
    }
    return next(err);
  }
}

export default new API();
