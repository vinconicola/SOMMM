function qs(sel) {
  return document.querySelector(sel);
}
// and here's the trick (works everywhere)
function g(f) {
  /in/.test(document.readyState) ? setTimeout("g(" + f + ")", 9) : f();
}
async function displayIPSettings(isVisible) {
  qs("#indirizzo_ip").style.display = isVisible ? "" : "none";
  document.querySelectorAll("#indirizzo_ip input").forEach((userItem) => {
    userItem.required = isVisible;
    if(!isVisible) userItem.value = "";
  });
}
g(function () {
  // On document ready
  fetch("/config")
    .then((r) => {
      if (r.ok) return r.json();
      // Success!
      else return Promise.reject(`Error on /config - status ${r.status}`);
    })
    .then((configJson) => {
      qs("#net_ssid").value = configJson.net_ssid;
      qs("#aula").value = configJson.aula;
      qs("#attuale").textContent = configJson.aula;
      qs("#api_url").value = configJson.api_url;
      if (configJson.net_static != null) {
        Object.entries(configJson.net_static).forEach(
          ([key, value]) => (qs(`#${key}`).value = value.join("."))
        );
        qs("#net_static").checked = true;
        displayIPSettings(true);
      }
    })
    .catch((error) => {
      // There was a connection error of some sort
      console.error(error);
      alert(error);
    });

  qs("#send").addEventListener("submit", async function (e) {
    e.preventDefault(); //stop form from submitting
    document.body.style.cursor = "wait";
    qs("#salva").disabled = true;

    var params = {};
    var net_static;

    params.net_ssid = qs("#net_ssid").value;
    params.net_pswd = qs("#net_pswd").value;
    if (qs("#net_static").checked) {
      net_ip = qs("#net_ip").value.split(".");
      net_sm = qs("#net_sm").value.split(".");
      net_dfgw = qs("#net_dfgw").value.split(".");
      net_dns = qs("#net_dns").value.split(".");
      net_static = {
        net_ip,
        net_sm,
        net_dfgw,
        net_dns,
      };
    } else net_static = null;

    params.net_static = net_static;
    params.aula = qs("#aula").value;
    params.api_url = qs("#api_url").value;

    console.log(params);

    var r = await fetch("/save", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(params),
    });

    var data;
    if (r.ok) {
      // Success!
      data = await r.text();
      console.log(data);
    } else {
      // We reached our target server, but it returned an error (500)
      data = `Error ${r.statusText} - status ${r.status}`;
      console.error(data);
    }
    alert(data);
    document.body.style.cursor = "default";
    qs("#salva").disabled = false;
  });
});
