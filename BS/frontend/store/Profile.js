import Item from 'store/Item';
import {action, observable, autorun} from 'mobx';
import superagent from 'superagent';

export default class Profile extends Item {

  @observable retrieving = false;

  @observable username;
  @observable email;
  @observable avatar;

  @observable err;

  // more and more

  constructor() {
    super();
    autorun(() => {
      if (!Item.token) return;
      superagent.get(`${this.host}/api/user`)
        .use(this.tokenPlugin)
        .then(({body: {user}}) => this.update(user))
        .catch(action(err => this.err = err));

    });
  }

  @action update(info) {
    ({
      username: this.username,
      email: this.email,
      avatar: this.avatar
    } = info);
  }
}