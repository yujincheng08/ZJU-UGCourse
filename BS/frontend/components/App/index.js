import React from 'react';
import {Route} from 'react-router-dom'
import Home from 'components/Home';
import LoginRegister from 'components/Account';
import Word from 'components/Word';

export default class App extends React.Component {
  render() {
    return (
      <div id="app">
        <Route exact path="/" component={Home}/>
        <Route path="/:action(login|register)" component={LoginRegister}/>
        <Route path="/word/:word" component={Word}/>
      </div>
    );
  }
}
