import {action, observable} from 'mobx';
import CONFIG from 'store/default';
import DEV from 'store/dev';

class Store {
  @observable isAuth = false;

  constructor() {
    this.isAuth = CONFIG.isAuth;
    this.fetchConfig();
  }

  @action updateConfig(config) {
    if (config.isAuth)
      this.isAuth = config.isAuth;
  }

  fetchConfig() {
    let host = '';
    if (process.env.NODE_ENV !== 'production')
      host = `${DEV.protocol}://${DEV.host}:${DEV.port}`;
    fetch(`${host}/config`)
      .then(response => response.json())
      .then(json => this.updateConfig(json));
  }
}

const STORE = new Store();

export default STORE;
