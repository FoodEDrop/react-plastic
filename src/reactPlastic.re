type state = {
  cardNumber: option(string),
  expiration: option(string),
  zipCode: option(int),
  cvc: option(int),
  creditCardType: option(Plastic.t),
};

type action =
  | ChangeCardNumber(option(string), option(Plastic.t))
  | ChangeExpiration(option(string))
  | ChangeZipCode(option(int))
  | ChangeCVCCode(option(int));

[@bs.deriving abstract]
type childrenProps = {
  cardNumber: string,
  expiration: string,
  zipCode: string,
  cvc: string,
  creditCardType: string,
  getCardNumberForDisplay: string => string,
  getExpirationForDisplay: string => string,
  handleCvcChange: ReactEvent.Form.t => unit,
  handleZipCodeChange: ReactEvent.Form.t => unit,
  handleExpirationChange: ReactEvent.Form.t => unit,
  handleCreditCardChange: ReactEvent.Form.t => unit,
  handleExpirationKeyDown: ReactEvent.Keyboard.t => unit,
};

let unwrapString = str =>
  switch (str) {
  | Some(value) => value
  | None => ""
  };

let unwrapInt = int =>
  switch (int) {
  | Some(value) => string_of_int(value)
  | None => ""
  };

/* Display utilities */
let rec divideIntoGroupsOfFour = str =>
  if (str->String.length <= 4) {
    [str];
  } else {
    [
      str->String.sub(0, 4),
      ...divideIntoGroupsOfFour(str->String.sub(4, str->String.length - 4)),
    ];
  };

let getCardNumberForDisplay = creditCardNumber =>
  creditCardNumber
  ->divideIntoGroupsOfFour
  ->Belt.List.reduce("", (str, section) => str ++ " " ++ section)
  |> String.trim;

let getExpirationForDisplay = expiration =>
  if (expiration->String.length >= 2 && !expiration->String.contains('/')) {
    expiration->String.sub(0, 2)
    ++ " / "
    ++ expiration->String.sub(2, expiration->String.length - 2);
  } else {
    expiration;
  };

let handleCreditCardChange = (e, {ReasonReact.send}) => {
  let value =
    ReactEvent.Form.target(e)##value
    |> [%re {|/\s/g|}]->Js.String.replaceByRe("");
  let valueLength = value->String.length;
  if (valueLength <= 16) {
    send(ChangeCardNumber(Some(value), Plastic.getCardType(value)));
  };
};

let handleExpirationChange = (e, {ReasonReact.send}) => {
  let rawValue =
    ReactEvent.Form.target(e)##value
    |> [%re {|/\s\/\s?/g|}]->Js.String.replaceByRe("");
  let rawValueLength = rawValue->String.length;
  let value =
    switch (rawValueLength) {
    | length when length >= 2 =>
      Js.Re.fromString("^(0[1-9]|1[012])") |> Js.Re.test(rawValue) ?
        rawValue->String.trim : rawValue->String.sub(0, 1)
    | length when length == 1 =>
      rawValue->String.sub(0, 1)->int_of_string > 1 ?
        "0" ++ rawValue : rawValue->String.trim
    | _ => ""
    };
  let valueLength = value->String.length;

  if (valueLength <= 4) {
    send(ChangeExpiration(Some(value)));
  };
};

let handleExpirationKeyDown = (e, {ReasonReact.state, send}) => {
  let currentState =
    switch (state.expiration) {
    | Some(value) => value
    | None => ""
    };
  switch (ReactEvent.Keyboard.key(e), currentState->String.length) {
  | ("Backspace", expLength) =>
    switch (expLength) {
    | 2 =>
      send(
        ChangeExpiration(
          Some(currentState->String.sub(0, expLength - 1) ++ " "),
        ),
      )
    | _ => ()
    }
  | _ => ()
  };
};

let handleCvcChange = (e, {ReasonReact.state, send}) => {
  let value = ReactEvent.Form.target(e)##value;
  let valueLength = value->String.length;
  if (valueLength <= Plastic.getNumberOfCvcDigits(state.creditCardType)) {
    send(ChangeCVCCode(Some(value)));
  };
};
let handleZipCodeChange = (e, {ReasonReact.state: _state, send}) => {
  let value = ReactEvent.Form.target(e)##value->String.trim;
  let valueLength = value->String.length;
  if (valueLength >= 1 && valueLength <= 5) {
    send(ChangeZipCode(Some(value->int_of_string)));
  } else if (valueLength < 1) {
    send(ChangeZipCode(None));
  };
};

let component = ReasonReact.reducerComponent("ReactPlastic");

let make = children => {
  ...component,
  initialState: () => {
    cardNumber: None,
    expiration: None,
    zipCode: None,
    cvc: None,
    creditCardType: None,
  },
  reducer: (action, state) =>
    switch (action) {
    | ChangeCardNumber(cardNumber, creditCardType) =>
      ReasonReact.Update({...state, cardNumber, creditCardType})
    | ChangeExpiration(expiration) =>
      ReasonReact.Update({...state, expiration})
    | ChangeZipCode(zipCode) => ReasonReact.Update({...state, zipCode})
    | ChangeCVCCode(cvc) => ReasonReact.Update({...state, cvc})
    },
  render:
    (
      {state: {cardNumber, expiration, zipCode, creditCardType, cvc}, handle},
    ) =>
    <div>
      {
        children(
          childrenProps(
            ~cardNumber=unwrapString(cardNumber),
            ~expiration=unwrapString(expiration),
            ~zipCode=unwrapInt(zipCode),
            ~cvc=unwrapInt(cvc),
            ~creditCardType=Plastic.getTypeString(creditCardType),
            ~getCardNumberForDisplay,
            ~getExpirationForDisplay,
            ~handleCreditCardChange=handle(handleCreditCardChange),
            ~handleCvcChange=handle(handleCvcChange),
            ~handleExpirationChange=handle(handleExpirationChange),
            ~handleZipCodeChange=handle(handleZipCodeChange),
            ~handleExpirationKeyDown=handle(handleExpirationKeyDown),
          ),
        )
      }
    </div>,
};