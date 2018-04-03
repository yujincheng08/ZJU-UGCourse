import passport from 'passport'
import {Strategy} from 'passport-local';
import User from 'models/User';

const field = {
  usernameField: 'user[email]',
  passwordField: 'user[password]'
};

class PassPort {
  passport = passport;

  constructor() {
    this.passport.use(new Strategy(field, (...args) => this.check(...args)));
  }

  check(email, password, done) {
    User.findOne({email: email}).then(function (user) {
      if (!user || !user.validPassword(password)) {
        return done(null, false, {errors: {'email or password': 'is invalid'}});
      }

      return done(null, user);
    }).catch(done);
  }

  authenticate(callback) {
    return this.passport.authenticate('local', {session: false}, callback);
  }
}

const Passport = new PassPort();
export default Passport;


