import Profile from 'store/Profile';
import Auth from 'store/Auth';

class Store {
  profile = new Profile;
  auth = new Auth;
}

const STORE = new Store();

export default STORE;
