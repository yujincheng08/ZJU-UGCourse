import React from 'react';
import TextField from 'material-ui/TextField';
import Button from 'material-ui/Button';
import {inject, observer} from 'mobx-react';
import PasswordField from 'components/common/PasswordField';
import {withRouter} from 'react-router-dom';

@inject('store')
@withRouter
@observer
export default class Login extends React.Component {
  onSubmit = (event) => {
    event.preventDefault();
    this.props.store.auth.login()
      .then(() => this.props.history.replace('/'));
  };

  componentWillUnmount() {
    this.props.store.auth.reset();
  }

  render() {
    const {auth} = this.props.store;
    const {values} = auth;
    return (
      <form onSubmit={this.onSubmit}>
        <TextField
          label={"Email"}
          fullWidth required
          inputProps={{pattern: '\\S+@\\S+\\.\\S+'}}
          onChange={e => auth.setEmail(e.target.value)}
          value={values.email}
        />
        <PasswordField
          label={"Password"}
          fullWidth required
          onChange={e => auth.setPassword(e.target.value)}
          value={values.password}
        />
        <Button variant={"raised"}
                type={"submit"}
                fullWidth
        >Login</Button>
        {auth.error === undefined ? null :
          <span>{auth.error.toString()}</span>
        }
      </form>
    )
  }
}