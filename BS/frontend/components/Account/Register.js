import {action, autorun, observable} from 'mobx';
import {inject, observer} from 'mobx-react';
import React from 'react';
import TextField from 'material-ui/TextField';
import Button from 'material-ui/Button';
import PasswordField from 'components/common/PasswordField';
import {withRouter} from 'react-router-dom';

@inject('store')
@withRouter
@observer
export default class Register extends React.Component {

  @observable confirmedPassword = '';
  @observable password = '';

  componentWillMount() {
    autorun(() => {
      if (this.password === this.confirmedPassword && this.password.length > 0)
        this.props.store.auth.setPassword(this.password);
    });
  }

  onSubmit = (event) => {
    event.preventDefault();
    this.props.store.auth.register()
      .then(() => this.props.history.replace('/'));
  };

  componentWillUnmount() {
    this.props.store.auth.reset();
  }

  render() {
    const {auth} = this.props.store;
    const {values, progressing} = auth;
    return (
      <form onSubmit={this.onSubmit}>
        <TextField label={"Username"}
                   fullWidth required
                   value={values.username}
                   onChange={e => {
                     auth.setUsername(e.target.value);
                   }}
        />
        <TextField label={"Email"}
                   fullWidth required
                   inputProps={{pattern: '\\S+@\\S+\\.\\S+'}}
                   value={values.email}
                   onChange={e => {
                     auth.setEmail(e.target.value);
                   }}
        />
        <PasswordField label={"Password"}
                       fullWidth required
                       onChange={
                         action(e => this.password = e.target.value)
                       }
                       value={this.password}
        />
        <PasswordField label={"Confirm password"}
                       fullWidth required
                       onChange={
                         action(e => this.confirmedPassword = e.target.value)
                       }
                       value={this.confirmedPassword}
        />
        <Button variant={"raised"}
                type={"submit"}
                fullWidth
                disabled={progressing}
        >Register</Button>
      </form>
    );
  }
}
