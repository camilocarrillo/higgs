#/bin/bash
export cut0=all_hlt_dipho
export cut1=EBPV
export cut2=EBPV^presel_EBPV
export cut3=ANDPV
export cut4=ANDPV^presel_ANDPV
export cut5=ANDPVorEBPV
export cut6=ANDPVorEBPV^presel_ANDPVorEBPV
export cut7=redisc
export cut8=redisc^presel_redisc



cat index.template | sed -e "s|cut\[0\]|$cut0|g" | sed -e "s|cut\[1\]|$cut1|g" | sed -e "s|cut\[2\]|$cut2|g" | sed -e "s|cut\[3\]|$cut3|g" | sed -e "s|cut\[4\]|$cut4|g" | sed -e "s|cut\[5\]|$cut5|g" | sed -e "s|cut\[6\]|$cut6|g" | sed -e "s|cut\[7\]|$cut7|g" | sed -e "s|cut\[8\]|$cut8|g" > buff.html
cat buff.html      | sed -e "s|\[0\]|$cut0|g" | sed -e "s|\[1\]|$cut1|g" | sed -e "s|\[2\]|$cut2|g" | sed -e "s|\[3\]|$cut3|g" | sed -e "s|\[4\]|$cut4|g" | sed -e "s|\[5\]|$cut5|g" | sed -e "s|\[6\]|$cut6|g" | sed -e "s|\[7\]|$cut7|g" | sed -e "s|\[8\]|$cut8|g" > index.html
rm buff.html

cat make_up.template | sed -e "s|cut\[0\]|$cut0|g" | sed -e "s|cut\[1\]|$cut1|g" | sed -e "s|cut\[2\]|$cut2|g" | sed -e "s|cut\[3\]|$cut3|g" | sed -e "s|cut\[4\]|$cut4|g" | sed -e "s|cut\[5\]|$cut5|g" | sed -e "s|cut\[6\]|$cut6|g" | sed -e "s|cut\[7\]|$cut7|g" | sed -e "s|cut\[8\]|$cut8|g" > make_up.C

