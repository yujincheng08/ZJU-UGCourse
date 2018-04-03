import Auth from 'Config/Auth';
import {Router as ExpressRouter} from 'express';
import User from 'models/User';
import Passport from 'Config/Passport';


class Users extends ExpressRouter {
  constructor() {
    super();
    this.get('/user', Auth.required, Users.getUser);
    this.post('/login', Users.login);
    this.put('/register', Users.register);
  }

  static getUser(req, res, next) {
    User.findById(req.user.id).then((user) => {
      if (!user)
        return res.sendStatus(401);
      return res.json({user: user.toAuthJSON()});
    }).catch(next);
  }

  static login(req, res, next) {
    const {email, password} = req.body.user;
    if (!email) {
      return res.status(422).json({
        errors: {
          email: "cannot be blank",
        },
      });
    }
    if (!password) {
      return res.status(422).json({
        errors: {
          password: "cannot be blank",
        }
      });
    }
    Passport.authenticate((err, user, info) => {
      if (err) {
        return next(err);
      }

      if (user) {
        user.token = user.generateJWT();
        return res.json({user: user.toAuthJSON()});
      } else {
        return res.status(422).json(info);
      }
    })(req, res, next);
  }

  static register(req, res, next) {
    let user = new User();

    user.username = req.body.user.username;
    user.email = req.body.user.email;
    user.setPassword(req.body.user.password);

    user.save().then(() => {
      return res.json({user: user.toAuthJSON()});
    }).catch(next);
  }
}

export default new Users();