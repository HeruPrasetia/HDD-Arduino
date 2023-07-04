export const numberFormat = (val) => {
    if (!isNaN(val)) {
        let num = String(val).split(".");
        let formatter = new Intl.NumberFormat("en-GB", { style: "decimal" });
        let result = formatter.format(num[0]);
        if (result != "NaN") {
            if (num.length > 1) {
                if (num[1] > 0) {
                    return result + "." + num[1];
                } else {
                    return result;
                }
            } else {
                return result;
            }
        } else {
            return "0";
        }
    } else {
        return "0";
    }
}

export const tanggalIndo = (data) => {
    let BulanIndo = ["Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agust", "Sept", "Okt", "Nov", "Des"];
    let tahun = data.substring(2, 4);
    let bulan = data.substring(5, 7);
    let tgl = data.substring(8, 10);
    let result = `${tgl} ${BulanIndo[bulan - 1]} ${tahun}`;
    return result;
}


// fungsi untuk mengambil data dari server
export const api = (url, data = []) => {
    if (data.length == 0) {
        return new Promise((resolve, reject) => {
            fetch(encodeURI(url))
                .then(response => response.text())
                .then(hasil => {
                    if (window.location.hostname == "localhost") {
                        if (isJson(hasil)) {
                            console.log(JSON.parse(hasil));
                        } else {
                            console.log(hasil);
                        }
                    }
                    try {
                        if (isJson(hasil)) {
                            resolve(JSON.parse(hasil));
                        } else {
                            resolve(hasil);
                        }
                    } catch (e) {
                        resolve({ status: "gagal", pesan: "Terjadi kesalahan ketika memproses" });
                    }
                }).catch((error) => {
                    reject({ status: "gagal", pesan: error });
                });
        });
    } else {
        return new Promise((resolve, reject) => {
            fetch(encodeURI(url), {
                method: 'POST',
                body: jsonToForm(data),
            })
                .then(response => response.text())
                .then(hasil => {
                    if (window.location.hostname == "localhost") {
                        if (isJson(hasil)) {
                            console.log(JSON.parse(hasil));
                        } else {
                            console.log(hasil);
                        }
                    }
                    try {
                        if (isJson(hasil)) {
                            resolve(JSON.parse(hasil));
                        } else {
                            resolve(hasil);
                        }
                    } catch (e) {
                        resolve({ status: "gagal", pesan: "Terjadi kesalahan ketika memproses" });
                    }
                }).catch((error) => {
                    reject({ status: "gagal", pesan: error });
                });
        });
    }
}

export const setCookie = (cname, cvalue, exdays) => {
    var d = new Date();
    d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
    var expires = "expires=" + d.toUTCString();
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
}

export const getCookie = (cname) => {
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for (var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

export const isJson = (str) => {
    try {
        JSON.parse(str);
    } catch (e) {
        return false;
    }
    return true;
}

export const jsonToForm = (data) => {
    const formData = new FormData();
    for (let dt in data) {
        formData.append(dt, data[dt]);
    }
    return formData;
}