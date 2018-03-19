import React from 'react';
import {inject, observer} from 'mobx-react';

@inject('store')
@observer
export default class Login extends React.Component {
  render() {
    return (
      <div>
        <span>
          {this.props.store.isAuth.toString()}
        </span>
      </div>
    )
  }
}