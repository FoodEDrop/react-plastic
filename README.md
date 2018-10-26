# React Plastic 💳

Credit Card forms made easy in JS and ReasonML.

## API

ReactPlastic accepts no props except a function as a child. The following props are available to that function.

```reasonml
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
```

## Example in JS

Look in the /example/index.js for a good example of how to use ReactPlastic in JS.
