import {action, computed, observable, reaction} from 'mobx';
import DEV from 'store/dev';

export default class Item {
  @observable static token = window.localStorage.getItem('jwt');

  host = process.env.NODE_ENV === 'production' ? '' : `${DEV.protocol}://${DEV.host}:${DEV.port}`;

  @action
  setToken(token) {
    Item.token = token;
  }

  @computed get isAuth() {
    return Item.token !== null;
  }

  tokenPlugin = (agent) => {
    if (Item.token)
      agent.set('authorization', `Bearer ${Item.token}`)
  };

  constructor() {
    reaction(() => Item.token, token =>
      token ? window.localStorage.setItem('jwt', token)
        : window.localStorage.removeItem('jwt')
    )
  }
}