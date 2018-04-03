import Item from 'store/Item';
import {action, observable} from 'mobx';
import superagent from 'superagent';

export default class Auth extends Item {
  @observable progressing = false;

  @observable error = undefined;

  @observable values = {
    username: '',
    email: '',
    password: '',
  };

  @action setUsername(username) {
    this.values.username = username;
  }

  @action setEmail(email) {
    this.values.email = email;
  }

  @action setPassword(password) {
    this.values.password = password;
  }

  @action reset() {
    this.values.username = '';
    this.values.password = '';
    this.values.email = '';
  }


  @action throwError() {

  }

  @action login() {
    this.progressing = true;
    this.error = undefined;
    const {email, password} = this.values;

    return superagent.post(`${this.host}/api/login`)
      .send({user: {email, password}})
      .then(({body: {user}}) => {
        this.setToken(user.token);
        this.reset();
      })
      .catch(action(err => this.error = err))
      .finally(action(() => this.progressing = false));
  }

  @action register() {
    this.progressing = true;
    this.error = undefined;
    const {username, email, password} = this.values;
    return superagent.put(`${this.host}/api/register`)
      .send({user: {username, email, password}})
      .then(({body: {user}}) => {
        this.setToken(user.token);
        this.reset();
      }).catch(action(err => this.error = err))
      .finally(action(() => this.progressing = false));
  }

  @action logout() {
    this.setToken(undefined);
    return Promise.resolve();
  }
}
