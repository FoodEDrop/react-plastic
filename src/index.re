type jsProps = {
  children: Js.t(ReactPlastic.childrenProps) => ReasonReact.reactElement,
};

let default =
  ReasonReact.wrapReasonForJs(~component=ReactPlastic.component, jsProps =>
    ReactPlastic.make(jsProps##children)
  );