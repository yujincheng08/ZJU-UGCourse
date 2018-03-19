import {render} from 'react-dom';
import {Provider} from 'mobx-react';
import {BrowserRouter as Router} from 'react-router-dom';
import React from 'react';
import Header from 'components/Header';
import App from 'components/App';
import STORE from 'store';

render(
  <Router>
    <Provider store={STORE}>
      <div id={"main"}>
        <Header/>
        <App/>
      </div>
    </Provider>
  </Router>
  ,
  document.getElementById('root')
);

