import jwt from 'express-jwt';
import {secret} from "Config";

export default class Auth {
  static required = jwt({
    secret: secret,
  });

  static optional = jwt({
    secret: secret,
    credentialsRequired: false,
  });

}