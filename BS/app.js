import {secret} from 'Config';
import Router from 'Router';
import Express from 'express';
import session from 'express-session';
import cors from 'cors';
import methodOverride from 'method-override';
import bodyParser from 'body-parser';
import mongoose from 'mongoose';

class App extends Express {
  PORT = process.env.PORT || 3000;


  static MONGODB = "mongodb://localhost/test";

  constructor() {
    super();
    this.use(cors());
    this.use(methodOverride());
    this.use(bodyParser.urlencoded({extended: false}));
    this.use(bodyParser.json());
    this.use(session({
      secret: secret,
      resave: false,
      saveUninitialized: false,
    }));


    this.use(Router);

    mongoose.connect(App.MONGODB);
    this.listen(this.PORT, this.listening);
  }

  listening = () => {
    console.log(`listening on ${this.PORT}`)
  };

}

new App();
