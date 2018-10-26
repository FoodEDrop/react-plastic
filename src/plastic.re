type t =
  | Visa
  | Amex
  | MasterCard
  | Discover;

let getTypeString = optionType =>
  switch (optionType) {
  | Some(value) =>
    switch (value) {
    | Visa => "visa"
    | Amex => "amex"
    | MasterCard => "mastercard"
    | Discover => "discover"
    }
  | None => ""
  };

let getCardType = cardNumber =>
  switch (cardNumber) {
  | num when Js.Re.fromString("^4") |> Js.Re.test(num) => Some(Visa)
  | num
      when
        Js.Re.fromString(
          "/^(5[1-5][0-9]{14}|2(22[1-9][0-9]{12}|2[3-9][0-9]{13}|[3-6][0-9]{14}|7[0-1][0-9]{13}|720[0-9]{12}))$/",
        )
        |> Js.Re.test(num) =>
    Some(MasterCard)
  | num when Js.Re.fromString("^3[47]") |> Js.Re.test(num) => Some(Amex)
  | num
      when
        Js.Re.fromString(
          "^(6011|622(12[6-9]|1[3-9][0-9]|[2-8][0-9]{2}|9[0-1][0-9]|92[0-5]|64[4-9])|65)",
        )
        |> Js.Re.test(num) =>
    Some(Discover)
  | _ => None
  };

let getNumberOfCvcDigits = cardTypeOption =>
  switch (cardTypeOption) {
  | Some(cardType) =>
    switch (cardType) {
    | Amex => 4
    | _ => 3
    }
  | None => 4
  };