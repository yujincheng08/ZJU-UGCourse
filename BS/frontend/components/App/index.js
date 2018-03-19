import React from 'react';
import {Route} from 'react-router-dom'
import Home from 'components/Home';
import Login from 'components/Login';
import Register from 'components/Register';
import Word from 'components/Word';

export default class App extends React.Component {
  render() {
    return (
      <div id="app">
        <Route exact path="/" component={Home}/>
        <Route path="/login" component={Login}/>
        <Route path="/register" component={Register}/>
        <Route path="/word/:word" component={Word}/>
      </div>
    );
  }
}
