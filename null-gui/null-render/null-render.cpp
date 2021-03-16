#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "../utils/utils.h"
#include "../helpers/math.h"
#include "null-render.h"
#include "../null-gui/null-gui.h"

const char* get_default_compressed_font_data() {
    char proggy_clean_ttf_compressed_data_base85[11980 + 1] =
        "7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
        "2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
        "`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
        "i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
        "kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
        "*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
        "tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
        "ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
        "x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
        "CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
        "U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
        "'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
        "_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
        "Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
        "/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
        "%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
        "OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
        "h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
        "o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
        "j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
        "sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
        "eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
        "M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
        "LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
        "%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
        "Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
        "a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
        "$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
        "nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
        "7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
        ")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
        "D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
        "P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
        "bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
        "h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
        "V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
        "sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
        ".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
        "$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
        "hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
        "@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
        "w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
        "u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
        "d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
        "6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
        "b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
        ":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
        "tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
        "$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
        ":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
        "7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
        "u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
        "LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
        ":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
        "_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
        "hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
        "^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
        "+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
        "9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
        "CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
        "hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
        "8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
        "S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
        "0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
        "+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
        "M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
        "?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
        "Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
        ">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
        "[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
        "wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
        "Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
        "MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
        "i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
        "1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
        "iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
        "URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
        ";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
        "w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
        "d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
        "A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
        "/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
        "m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
        "TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
        "GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
        "O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";

    return proggy_clean_ttf_compressed_data_base85;
}

unsigned int decode85_byte(char c) { return c >= '\\' ? c - 36 : c - 35; }
void decode85(const unsigned char* src, unsigned char* dst) {
    while (*src) {
        unsigned int tmp = decode85_byte(src[0]) + 85 * (decode85_byte(src[1]) + 85 * (decode85_byte(src[2]) + 85 * (decode85_byte(src[3]) + 85 * decode85_byte(src[4]))));
        dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);
        src += 5;
        dst += 4;
    }
}

#define stb_in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb_in3(x)   ((i[x] << 16) + stb_in2((x)+1))
#define stb_in4(x)   ((i[x] << 24) + stb_in3((x)+1))

namespace stb {
    unsigned char* barrier_out_e, *barrier_out_b;
    const unsigned char* barrier_in_b;
    unsigned char* dout;

    unsigned int decompress_length(const unsigned char* input) { return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11]; }
    void match(const unsigned char* data, unsigned int length) {
        assert(dout + length <= barrier_out_e);
        if (dout + length > barrier_out_e) { dout += length; return; }
        if (data < barrier_out_b) { dout = barrier_out_e + 1; return; }
        while (length--) *dout++ = *data++;
    }

    void lit(const unsigned char* data, unsigned int length) {
        assert(dout + length <= barrier_out_e);
        if (dout + length > barrier_out_e) { dout += length; return; }
        if (data < barrier_in_b) { dout = barrier_out_e + 1; return; }
        memcpy(dout, data, length);
        dout += length;
    }

    const unsigned char* decompress_token(const unsigned char* i) {
        if (*i >= 0x20) {
            if (*i >= 0x80)       match(dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
            else if (*i >= 0x40)  match(dout - (stb_in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
            else /* *i >= 0x20 */ lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
        } else {
            if (*i >= 0x18)       match(dout - (stb_in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
            else if (*i >= 0x10)  match(dout - (stb_in3(0) - 0x100000 + 1), stb_in2(3) + 1), i += 5;
            else if (*i >= 0x08)  lit(i + 2, stb_in2(0) - 0x0800 + 1), i += 2 + (stb_in2(0) - 0x0800 + 1);
            else if (*i == 0x07)  lit(i + 3, stb_in2(1) + 1), i += 3 + (stb_in2(1) + 1);
            else if (*i == 0x06)  match(dout - (stb_in3(1) + 1), i[4] + 1), i += 5;
            else if (*i == 0x04)  match(dout - (stb_in3(1) + 1), stb_in2(4) + 1), i += 6;
        }
        return i;
    }

    unsigned int adler32(unsigned int adler32, unsigned char* buffer, unsigned int buflen) {
        const unsigned long ADLER_MOD = 65521;
        unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
        unsigned long blocklen = buflen % 5552;

        unsigned long i;
        while (buflen) {
            for (i = 0; i + 7 < blocklen; i += 8) {
                s1 += buffer[0], s2 += s1;
                s1 += buffer[1], s2 += s1;
                s1 += buffer[2], s2 += s1;
                s1 += buffer[3], s2 += s1;
                s1 += buffer[4], s2 += s1;
                s1 += buffer[5], s2 += s1;
                s1 += buffer[6], s2 += s1;
                s1 += buffer[7], s2 += s1;

                buffer += 8;
            }

            for (; i < blocklen; ++i)
                s1 += *buffer++, s2 += s1;

            s1 %= ADLER_MOD, s2 %= ADLER_MOD;
            buflen -= blocklen;
            blocklen = 5552;
        }
        return (unsigned int)(s2 << 16) + (unsigned int)s1;
    }

    unsigned int decompress(unsigned char* output, const unsigned char* i, unsigned int /*length*/) {
        if (stb_in4(0) != 0x57bC0000) return 0;
        if (stb_in4(4) != 0) return 0;
        const unsigned int olen = stb::decompress_length(i);
        barrier_in_b = i;
        barrier_out_e = output + olen;
        barrier_out_b = output;
        i += 16;

        dout = output;
        for (;;) {
            const unsigned char* old_i = i;
            i = decompress_token(i);
            if (i == old_i) {
                if (*i == 0x05 && i[1] == 0xfa) {
                    assert(dout == output + olen);
                    if (dout != output + olen) return 0;
                    if (adler32(1, output, olen) != (unsigned int)stb_in4(2))
                        return 0;
                    return olen;
                } else {
                    assert(0); /* NOTREACHED */
                    return 0;
                }
            }
            assert(dout <= output + olen);
            if (dout > output + olen)
                return 0;
        }
    }
}

namespace null_font {
    namespace helpers {
        font_config::font_config() {
            font_data = NULL;
            font_data_size = 0;
            font_data_owned_by_atlas = true;
            font_no = 0;
            size_pixels = 0.0f;
            oversample_h = 3;
            oversample_v = 1;
            pixel_snap_h = false;
            glyph_extra_spacing = vec2(0.0f, 0.0f);
            glyph_offset = vec2(0.0f, 0.0f);
            glyph_ranges = NULL;
            glyph_min_advance_x = 0.0f;
            glyph_max_advance_x = FLT_MAX;
            merge_mode = false;
            rasterizer_flags = 0x00;
            rasterizer_multiply = 1.0f;
            ellipsis_char = (unsigned short)-1;
            dst_font = NULL;
        }

        void glyph_ranges_builder::add_text(const char* text, const char* text_end) {
            while (text_end ? (text < text_end) : *text) {
                unsigned int c = 0;
                int c_len = get_char_from_utf8(&c, text, text_end);
                text += c_len;
                if (c_len == 0)
                    break;
                add_char((unsigned short)c);
            }
        }

        void glyph_ranges_builder::add_ranges(const unsigned short* ranges) {
            for (; ranges[0]; ranges += 2)
                for (unsigned short c = ranges[0]; c <= ranges[1]; c++)
                    add_char(c);
        }

        void glyph_ranges_builder::build_ranges(std::vector<unsigned short>* out_ranges) {
            const int max_codepoint = 0xFFFF;
            for (int n = 0; n <= max_codepoint; n++)
                if (get_bit(n)) {
                    out_ranges->push_back((unsigned short)n);
                    while (n < max_codepoint && get_bit(n + 1))
                        n++;
                    out_ranges->push_back((unsigned short)n);
                }
            out_ranges->push_back(0);
        }

        atlas::atlas() {
            locked = false;
            tex_id = (void*)NULL;
            tex_desired_width = 0;
            tex_glyph_padding = 1;

            tex_pixels_alpha8 = NULL;
            tex_pixels_rgba32 = NULL;
            tex_width = tex_height = 0;
            tex_uv_scale = vec2(0.0f, 0.0f);
            tex_uv_white_pixel = vec2(0.0f, 0.0f);
            pack_id_mouse_cursors = pack_id_lines = -1;
        }

        atlas::~atlas() {
            assert(!locked && "Cannot modify a locked atlas between begin_render() and end_render/render()!");
            clear();
        }

        font* atlas::add_font(const font_config* font_cfg) {
            assert(!locked && "Cannot modify a locked atlas between begin_render() and end_render/render()!");
            assert(font_cfg->font_data != NULL && font_cfg->font_data_size > 0);
            assert(font_cfg->size_pixels > 0.0f);

            if (!font_cfg->merge_mode) fonts.push_back(new font);
            else assert(!fonts.empty() && "Cannot use merge_mode for the first font");

            config_data.push_back(*font_cfg);
            font_config& new_font_cfg = config_data.back();
            if (new_font_cfg.dst_font == NULL)
                new_font_cfg.dst_font = fonts.back();
            if (!new_font_cfg.font_data_owned_by_atlas) {
                new_font_cfg.font_data = malloc(new_font_cfg.font_data_size);
                new_font_cfg.font_data_owned_by_atlas = true;
                memcpy(new_font_cfg.font_data, font_cfg->font_data, (size_t)new_font_cfg.font_data_size);
            }

            if (new_font_cfg.dst_font->ellipsis_char == (unsigned short)-1)
                new_font_cfg.dst_font->ellipsis_char = font_cfg->ellipsis_char;

            clear_tex_data();
            return new_font_cfg.dst_font;
        }

        font* atlas::add_font_default(const font_config* font_cfg_template) {
            font_config font_cfg = font_cfg_template ? *font_cfg_template : font_config();
            if (!font_cfg_template) {
                font_cfg.oversample_h = font_cfg.oversample_v = 1;
                font_cfg.pixel_snap_h = true;
            }
            if (font_cfg.size_pixels <= 0.0f) font_cfg.size_pixels = 13.0f * 1.0f;
            font_cfg.ellipsis_char = (unsigned short)0x0085;
            font_cfg.glyph_offset.y = 1.0f * floor(font_cfg.size_pixels / 13.0f);

            const char* ttf_compressed_base85 = get_default_compressed_font_data();
            const unsigned short* glyph_ranges = font_cfg.glyph_ranges != NULL ? font_cfg.glyph_ranges : glyph_ranges_default();
            font* font = add_font_from_memory_compressed_base_85_ttf(ttf_compressed_base85, font_cfg.size_pixels, &font_cfg, glyph_ranges);
            return font;
        }

        font* atlas::add_font_from_file_ttf(const char* filename, float size_pixels, const font_config* font_cfg_template, const unsigned short* glyph_ranges) {
            assert(!locked && "Cannot modify a locked atlas between begin_render() and end_render/render()!");
            size_t data_size = 0;
            void* data = utils::file_load_to_memory(filename, "rb", &data_size, 0);
            if (!data) {
                assert((0) && "Could not load font file!");
                return NULL;
            }
            font_config font_cfg = font_cfg_template ? *font_cfg_template : font_config();
            return add_font_from_memory_ttf(data, (int)data_size, size_pixels, &font_cfg, glyph_ranges);
        }

        font* atlas::add_font_from_memory_ttf(void* ttf_data, int ttf_size, float size_pixels, const font_config* font_cfg_template, const unsigned short* glyph_ranges) {
            assert(!locked && "Cannot modify a locked atlas between begin_render() and end_render/render()!");
            font_config font_cfg = font_cfg_template ? *font_cfg_template : font_config();
            assert(font_cfg.font_data == NULL);
            font_cfg.font_data = ttf_data;
            font_cfg.font_data_size = ttf_size;
            font_cfg.size_pixels = size_pixels;
            if (glyph_ranges) font_cfg.glyph_ranges = glyph_ranges;
            return add_font(&font_cfg);
        }

        font* atlas::add_font_from_memory_compressed_ttf(const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, const font_config* font_cfg_template, const unsigned short* glyph_ranges) {
            const unsigned int buf_decompressed_size = stb::decompress_length((const unsigned char*)compressed_ttf_data);
            unsigned char* buf_decompressed_data = (unsigned char*)malloc(buf_decompressed_size);
            stb::decompress(buf_decompressed_data, (const unsigned char*)compressed_ttf_data, (unsigned int)compressed_ttf_size);

            font_config font_cfg = font_cfg_template ? *font_cfg_template : font_config();
            assert(font_cfg.font_data == NULL);
            font_cfg.font_data_owned_by_atlas = true;
            return add_font_from_memory_ttf(buf_decompressed_data, (int)buf_decompressed_size, size_pixels, &font_cfg, glyph_ranges);
        }

        font* atlas::add_font_from_memory_compressed_base_85_ttf(const char* compressed_ttf_data_base85, float size_pixels, const font_config* font_cfg, const unsigned short* glyph_ranges) {
            int compressed_ttf_size = (((int)strlen(compressed_ttf_data_base85) + 4) / 5) * 4;
            void* compressed_ttf = malloc((size_t)compressed_ttf_size);
            decode85((const unsigned char*)compressed_ttf_data_base85, (unsigned char*)compressed_ttf);
            font* font = add_font_from_memory_compressed_ttf(compressed_ttf, compressed_ttf_size, size_pixels, font_cfg, glyph_ranges);
            free(compressed_ttf);
            return font;
        }

        void atlas::clear_input_data() {
            assert(!locked && "Cannot modify a locked atlas between begin_render() and end_render/render()!");
            for (int i = 0; i < config_data.size(); i++)
                if (config_data[i].font_data && config_data[i].font_data_owned_by_atlas) {
                    free(config_data[i].font_data);
                    config_data[i].font_data = NULL;
                }

            for (int i = 0; i < fonts.size(); i++)
                if (fonts[i]->config_data >= config_data.data() && fonts[i]->config_data < config_data.data() + config_data.size()) {
                    fonts[i]->config_data = NULL;
                    fonts[i]->config_data_count = 0;
                }
            config_data.clear();
            custom_rects.clear();
            pack_id_mouse_cursors = pack_id_lines = -1;
        }

        void atlas::clear_tex_data() {
            assert(!locked && "Cannot modify a locked atlas between begin_render() and end_render/render()!");
            if (tex_pixels_alpha8) free(tex_pixels_alpha8);
            if (tex_pixels_rgba32) free(tex_pixels_rgba32);
            tex_pixels_alpha8 = NULL;
            tex_pixels_rgba32 = NULL;
        }

        void atlas::clear_fonts() {
            assert(!locked && "Cannot modify a locked atlas between begin_render() and end_render/render()!");
            for (int i = 0; i < fonts.size(); i++) {
                fonts[i]->~font();
                free(fonts[i]);
            }
            fonts.clear();
        }

        void atlas::clear() {
            clear_input_data();
            clear_tex_data();
            clear_fonts();
        }

        bool atlas::build() {
            assert(!locked && "Cannot modify a locked atlas between begin_render() and end_render/render()!");
            return atlas_build_with_stb_truetype(this);
        }

        void atlas::get_tex_data_as_alpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel) {
            if (tex_pixels_alpha8 == NULL) {
                if (config_data.empty())
                    add_font_default();
                build();
            }

            *out_pixels = tex_pixels_alpha8;
            if (out_width) *out_width = tex_width;
            if (out_height) *out_height = tex_height;
            if (out_bytes_per_pixel) *out_bytes_per_pixel = 1;
        }

        void atlas::get_tex_data_as_rgba32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel) {
            if (!tex_pixels_rgba32) {
                unsigned char* pixels = NULL;
                get_tex_data_as_alpha8(&pixels, NULL, NULL);
                if (pixels) {
                    tex_pixels_rgba32 = (unsigned int*)malloc((size_t)tex_width * (size_t)tex_height * 4);
                    const unsigned char* src = pixels;
                    unsigned int* dst = tex_pixels_rgba32;
                    for (int n = tex_width * tex_height; n > 0; n--)
                        *dst++ = COL32(255, 255, 255, (unsigned int)(*src++));
                }
            }

            *out_pixels = (unsigned char*)tex_pixels_rgba32;
            if (out_width) *out_width = tex_width;
            if (out_height) *out_height = tex_height;
            if (out_bytes_per_pixel) *out_bytes_per_pixel = 4;
        }

        int atlas::add_custom_rect_regular(int width, int height) {
            assert(width > 0 && width <= 0xFFFF);
            assert(height > 0 && height <= 0xFFFF);
            atlas_custom_rect r;
            r.width = (unsigned short)width;
            r.height = (unsigned short)height;
            custom_rects.push_back(r);
            return custom_rects.size() - 1;
        }

        int atlas::add_custom_rect_font_glyph(font* font, unsigned short id, int width, int height, float advance_x, const vec2& offset) {
            assert(font != NULL);
            assert(width > 0 && width <= 0xFFFF);
            assert(height > 0 && height <= 0xFFFF);
            atlas_custom_rect r;
            r.width = (unsigned short)width;
            r.height = (unsigned short)height;
            r.glyph_id = id;
            r.glyph_advance_x = advance_x;
            r.glyph_offset = offset;
            r.font = font;
            custom_rects.push_back(r);
            return custom_rects.size() - 1;
        }

        void atlas::calc_custom_rect_uv(const atlas_custom_rect* rect, vec2* out_uv_min, vec2* out_uv_max) const {
            assert(tex_width > 0 && tex_height > 0);
            assert(rect->is_packed());
            *out_uv_min = vec2((float)rect->x * tex_uv_scale.x, (float)rect->y * tex_uv_scale.y);
            *out_uv_max = vec2((float)(rect->x + rect->width) * tex_uv_scale.x, (float)(rect->y + rect->height) * tex_uv_scale.y);
        }

        int get_char_from_utf8(unsigned int* out_char, const char* in_text, const char* in_text_end) {
            static const char lengths[32] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0 };
            static const int masks[] = { 0x00, 0x7f, 0x1f, 0x0f, 0x07 };
            static const uint32_t mins[] = { 0x400000, 0, 0x80, 0x800, 0x10000 };
            static const int shiftc[] = { 0, 18, 12, 6, 0 };
            static const int shifte[] = { 0, 6, 4, 2, 0 };
            int len = lengths[*(const unsigned char*)in_text >> 3];
            int wanted = len + !len;

            if (in_text_end == NULL)
                in_text_end = in_text + wanted;

            unsigned char s[4];
            s[0] = in_text + 0 < in_text_end ? in_text[0] : 0;
            s[1] = in_text + 1 < in_text_end ? in_text[1] : 0;
            s[2] = in_text + 2 < in_text_end ? in_text[2] : 0;
            s[3] = in_text + 3 < in_text_end ? in_text[3] : 0;

            *out_char = (uint32_t)(s[0] & masks[len]) << 18;
            *out_char |= (uint32_t)(s[1] & 0x3f) << 12;
            *out_char |= (uint32_t)(s[2] & 0x3f) << 6;
            *out_char |= (uint32_t)(s[3] & 0x3f) << 0;
            *out_char >>= shiftc[len];

            int e = 0;
            e = (*out_char < mins[len]) << 6;
            e |= ((*out_char >> 11) == 0x1b) << 7;
            e |= (*out_char > 0xFFFF) << 8;
            e |= (s[1] & 0xc0) >> 2;
            e |= (s[2] & 0xc0) >> 4;
            e |= (s[3]) >> 6;
            e ^= 0x2a;
            e >>= shifte[len];

            if (e) {
                wanted = math::min(wanted, !!s[0] + !!s[1] + !!s[2] + !!s[3]);
                *out_char = 0xFFFD;
            }

            return wanted;
        }

        const unsigned short* glyph_ranges_default() {
            static const unsigned short ranges[] = {
                0x0020, 0x00FF,
                0,
            };
            return &ranges[0];
        }

        const unsigned short* glyph_ranges_korean() {
            static const unsigned short ranges[] = {
                0x0020, 0x00FF,
                0x3131, 0x3163,
                0xAC00, 0xD7A3,
                0,
            };
            return &ranges[0];
        }

        const unsigned short* glyph_ranges_chinese_full() {
            static const unsigned short ranges[] = {
                0x0020, 0x00FF,
                0x2000, 0x206F,
                0x3000, 0x30FF,
                0x31F0, 0x31FF,
                0xFF00, 0xFFEF,
                0x4e00, 0x9FAF,
                0,
            };
            return &ranges[0];
        }

        const unsigned short* glyph_ranges_cyrillic() {
            static const unsigned short ranges[] = {
                0x0020, 0x00FF,
                0x0400, 0x052F,
                0x2DE0, 0x2DFF,
                0xA640, 0xA69F,
                0,
            };
            return &ranges[0];
        }

        const unsigned short* glyph_ranges_thai() {
            static const unsigned short ranges[] = {
                0x0020, 0x00FF,
                0x2010, 0x205E,
                0x0E00, 0x0E7F,
                0,
            };
            return &ranges[0];
        }

        const unsigned short* glyph_ranges_vietnamese() {
            static const unsigned short ranges[] = {
                0x0020, 0x00FF,
                0x0102, 0x0103,
                0x0110, 0x0111,
                0x0128, 0x0129,
                0x0168, 0x0169,
                0x01A0, 0x01A1,
                0x01AF, 0x01B0,
                0x1EA0, 0x1EF9,
                0,
            };
            return &ranges[0];
        }

        void atlas_build_render_lines_tex_data(atlas* atlas) {
            atlas_custom_rect* r = atlas->get_custom_rect_by_index(atlas->pack_id_lines);
            assert(r->is_packed());
            for (unsigned int n = 0; n < 64; n++) {
                unsigned int y = n;
                unsigned int line_width = n;
                unsigned int pad_left = (r->width - line_width) / 2;
                unsigned int pad_right = r->width - (pad_left + line_width);

                assert(pad_left + line_width + pad_right == r->width && y < r->height);
                unsigned char* write_ptr = &atlas->tex_pixels_alpha8[r->x + ((r->y + y) * atlas->tex_width)];
                memset(write_ptr, 0x00, pad_left);
                memset(write_ptr + pad_left, 0xFF, line_width);
                memset(write_ptr + pad_left + line_width, 0x00, pad_right);

                vec2 uv0 = vec2((float)(r->x + pad_left - 1), (float)(r->y + y)) * atlas->tex_uv_scale;
                vec2 uv1 = vec2((float)(r->x + pad_left + line_width + 1), (float)(r->y + y + 1)) * atlas->tex_uv_scale;
                float half_v = (uv0.y + uv1.y) * 0.5f;
                atlas->tex_uv_lines[n] = rect(uv0.x, half_v, uv1.x, half_v);
            }
        }

        void atlas_build_render_default_tex_data(atlas* atlas) {
            atlas_custom_rect* r = atlas->get_custom_rect_by_index(atlas->pack_id_mouse_cursors);
            assert(r->is_packed());

            const int w = atlas->tex_width;
            assert(r->width == 2 && r->height == 2);
            const int offset = (int)r->x + (int)r->y * w;
            atlas->tex_pixels_alpha8[offset] = atlas->tex_pixels_alpha8[offset + 1] = atlas->tex_pixels_alpha8[offset + w] = atlas->tex_pixels_alpha8[offset + w + 1] = 0xFF;

            atlas->tex_uv_white_pixel = vec2((r->x + 0.5f) * atlas->tex_uv_scale.x, (r->y + 0.5f) * atlas->tex_uv_scale.y);
        }

        bool atlas_build_with_stb_truetype(atlas* atlas) {
            assert(atlas->config_data.size() > 0);

            atlas_build_init(atlas);

            atlas->tex_id = (void*)NULL;
            atlas->tex_width = atlas->tex_height = 0;
            atlas->tex_uv_scale = vec2(0.0f, 0.0f);
            atlas->tex_uv_white_pixel = vec2(0.0f, 0.0f);
            atlas->clear_tex_data();

            std::vector<src_data> src_tmp_array;
            std::vector<dst_data> dst_tmp_array;
            src_tmp_array.resize(atlas->config_data.size());
            dst_tmp_array.resize(atlas->fonts.size());
            memset(src_tmp_array.data(), 0, (size_t)(src_tmp_array.size() * (int)sizeof(src_data)));
            memset(dst_tmp_array.data(), 0, (size_t)(dst_tmp_array.size() * (int)sizeof(dst_data)));

            for (int src_i = 0; src_i < atlas->config_data.size(); src_i++) {
                src_data& src_tmp = src_tmp_array[src_i];
                font_config& cfg = atlas->config_data[src_i];
                assert(cfg.dst_font && (!cfg.dst_font->is_loaded() || cfg.dst_font->container_atlas == atlas));

                src_tmp.dst_index = -1;
                for (int output_i = 0; output_i < atlas->fonts.size() && src_tmp.dst_index == -1; output_i++)
                    if (cfg.dst_font == atlas->fonts[output_i])
                        src_tmp.dst_index = output_i;
                assert(src_tmp.dst_index != -1);
                if (src_tmp.dst_index == -1)
                    return false;

                const int font_offset = stbtt_GetFontOffsetForIndex((unsigned char*)cfg.font_data, cfg.font_no);
                assert(font_offset >= 0 && "font_data is incorrect, or font_no cannot be found.");
                if (!stbtt_InitFont(&src_tmp.font_info, (unsigned char*)cfg.font_data, font_offset))
                    return false;

                dst_data& dst_tmp = dst_tmp_array[src_tmp.dst_index];
                src_tmp.src_ranges = cfg.glyph_ranges ? cfg.glyph_ranges : glyph_ranges_default();
                for (const unsigned short* src_range = src_tmp.src_ranges; src_range[0] && src_range[1]; src_range += 2)
                    src_tmp.glyphs_highest = math::max(src_tmp.glyphs_highest, (int)src_range[1]);
                dst_tmp.src_count++;
                dst_tmp.glyphs_highest = math::max(dst_tmp.glyphs_highest, src_tmp.glyphs_highest);
            }

            int total_glyphs_count = 0;
            for (int src_i = 0; src_i < src_tmp_array.size(); src_i++) {
                src_data& src_tmp = src_tmp_array[src_i];
                dst_data& dst_tmp = dst_tmp_array[src_tmp.dst_index];
                src_tmp.glyphs_set.create(src_tmp.glyphs_highest + 1);
                if (dst_tmp.glyphs_set.storage.empty())
                    dst_tmp.glyphs_set.create(dst_tmp.glyphs_highest + 1);

                unsigned int temp_codepoint;
                for (const unsigned short* src_range = src_tmp.src_ranges; src_range[0] && src_range[1]; src_range += 2) {
                    for (unsigned int codepoint = src_range[0]; codepoint <= src_range[1]; codepoint++) {
                        if (dst_tmp.glyphs_set.test_bit(codepoint)) continue;
                        if (!stbtt_FindGlyphIndex(&src_tmp.font_info, codepoint)) continue;

                        src_tmp.glyphs_count++;
                        dst_tmp.glyphs_count++;
                        src_tmp.glyphs_set.set_bit(codepoint);
                        dst_tmp.glyphs_set.set_bit(codepoint);
                        total_glyphs_count++;
                        temp_codepoint = codepoint;
                    }
                }
            }

            for (int src_i = 0; src_i < src_tmp_array.size(); src_i++) {
                src_data& src_tmp = src_tmp_array[src_i];
                src_tmp.glyphs_list.reserve(src_tmp.glyphs_count);
                src_tmp.glyphs_set.unpack_bit_vector_to_flat_index_list(&src_tmp.glyphs_list);
                src_tmp.glyphs_set.clear();
                assert(src_tmp.glyphs_list.size() == src_tmp.glyphs_count);
            }
            for (int dst_i = 0; dst_i < dst_tmp_array.size(); dst_i++)
                dst_tmp_array[dst_i].glyphs_set.clear();
            dst_tmp_array.clear();

            std::vector<stbrp_rect> buf_rects;
            std::vector<stbtt_packedchar> buf_packedchars;
            buf_rects.resize(total_glyphs_count);
            buf_packedchars.resize(total_glyphs_count);
            memset(buf_rects.data(), 0, (size_t)(buf_rects.size() * (int)sizeof(stbrp_rect)));
            memset(buf_packedchars.data(), 0, (size_t)(buf_packedchars.size() * (int)sizeof(stbtt_packedchar)));

            int total_surface = 0;
            int buf_rects_out_n = 0;
            int buf_packedchars_out_n = 0;
            for (int src_i = 0; src_i < src_tmp_array.size(); src_i++) {
                src_data& src_tmp = src_tmp_array[src_i];
                if (src_tmp.glyphs_count == 0)
                    continue;

                src_tmp.rects = &buf_rects[buf_rects_out_n];
                src_tmp.packed_chars = &buf_packedchars[buf_packedchars_out_n];
                buf_rects_out_n += src_tmp.glyphs_count;
                buf_packedchars_out_n += src_tmp.glyphs_count;

                font_config& cfg = atlas->config_data[src_i];
                src_tmp.pack_range.font_size = cfg.size_pixels;
                src_tmp.pack_range.first_unicode_codepoint_in_range = 0;
                src_tmp.pack_range.array_of_unicode_codepoints = src_tmp.glyphs_list.data();
                src_tmp.pack_range.num_chars = src_tmp.glyphs_list.size();
                src_tmp.pack_range.chardata_for_range = src_tmp.packed_chars;
                src_tmp.pack_range.h_oversample = (unsigned char)cfg.oversample_h;
                src_tmp.pack_range.v_oversample = (unsigned char)cfg.oversample_v;

                const float scale = (cfg.size_pixels > 0) ? stbtt_scaleForPixelHeight(&src_tmp.font_info, cfg.size_pixels) : stbtt_scaleForMappingEmToPixels(&src_tmp.font_info, -cfg.size_pixels);
                const int padding = atlas->tex_glyph_padding;
                for (int glyph_i = 0; glyph_i < src_tmp.glyphs_list.size(); glyph_i++) {
                    int x0, y0, x1, y1;
                    const int glyph_index_in_font = stbtt_FindGlyphIndex(&src_tmp.font_info, src_tmp.glyphs_list[glyph_i]);
                    assert(glyph_index_in_font != 0);
                    stbtt_GetGlyphBitmapBoxSubpixel(&src_tmp.font_info, glyph_index_in_font, scale * cfg.oversample_h, scale * cfg.oversample_v, 0, 0, &x0, &y0, &x1, &y1);
                    src_tmp.rects[glyph_i].w = (stbrp_coord)(x1 - x0 + padding + cfg.oversample_h - 1);
                    src_tmp.rects[glyph_i].h = (stbrp_coord)(y1 - y0 + padding + cfg.oversample_v - 1);
                    total_surface += src_tmp.rects[glyph_i].w * src_tmp.rects[glyph_i].h;
                }
            }

            const int surface_sqrt = (int)sqrtf((float)total_surface) + 1;
            atlas->tex_height = 0;
            if (atlas->tex_desired_width > 0) atlas->tex_width = atlas->tex_desired_width;
            else atlas->tex_width = (surface_sqrt >= 4096 * 0.7f) ? 4096 : (surface_sqrt >= 2048 * 0.7f) ? 2048 : (surface_sqrt >= 1024 * 0.7f) ? 1024 : 512;

            const int TEX_HEIGHT_MAX = 1024 * 32;
            stbtt_pack_context spc = {};
            stbtt_PackBegin(&spc, NULL, atlas->tex_width, TEX_HEIGHT_MAX, 0, atlas->tex_glyph_padding, NULL);
            atlas_build_pack_custom_rects(atlas, spc.pack_info);

            for (int src_i = 0; src_i < src_tmp_array.size(); src_i++) {
                src_data& src_tmp = src_tmp_array[src_i];
                if (src_tmp.glyphs_count == 0)
                    continue;

                stbrp_pack_rects((stbrp_context*)spc.pack_info, src_tmp.rects, src_tmp.glyphs_count);

                for (int glyph_i = 0; glyph_i < src_tmp.glyphs_count; glyph_i++)
                    if (src_tmp.rects[glyph_i].was_packed)
                        atlas->tex_height = math::max(atlas->tex_height, src_tmp.rects[glyph_i].y + src_tmp.rects[glyph_i].h);
            }

            atlas->tex_height = upper_power_of_two(atlas->tex_height);
            atlas->tex_uv_scale = vec2(1.0f / atlas->tex_width, 1.0f / atlas->tex_height);
            atlas->tex_pixels_alpha8 = (unsigned char*)malloc(atlas->tex_width * atlas->tex_height);
            memset(atlas->tex_pixels_alpha8, 0, atlas->tex_width * atlas->tex_height);
            spc.pixels = atlas->tex_pixels_alpha8;
            spc.height = atlas->tex_height;

            for (int src_i = 0; src_i < src_tmp_array.size(); src_i++) {
                font_config& cfg = atlas->config_data[src_i];
                src_data& src_tmp = src_tmp_array[src_i];
                if (src_tmp.glyphs_count == 0)
                    continue;

                stbtt_PackFontRangesRenderIntoRects(&spc, &src_tmp.font_info, &src_tmp.pack_range, 1, src_tmp.rects);

                if (cfg.rasterizer_multiply != 1.0f) {
                    unsigned char multiply_table[256];
                    atlas_build_multiply_calc_lookup_table(multiply_table, cfg.rasterizer_multiply);
                    stbrp_rect* r = &src_tmp.rects[0];
                    for (int glyph_i = 0; glyph_i < src_tmp.glyphs_count; glyph_i++, r++)
                        if (r->was_packed)
                            atlas_build_multiply_rect_alpha8(multiply_table, atlas->tex_pixels_alpha8, r->x, r->y, r->w, r->h, atlas->tex_width * 1);
                }
                src_tmp.rects = NULL;
            }

            stbtt_PackEnd(&spc);
            buf_rects.clear();

            for (int src_i = 0; src_i < src_tmp_array.size(); src_i++) {
                src_data& src_tmp = src_tmp_array[src_i];
                if (src_tmp.glyphs_count == 0)
                    continue;

                font_config& cfg = atlas->config_data[src_i];
                font* dst_font = cfg.dst_font;

                const float font_scale = stbtt_scaleForPixelHeight(&src_tmp.font_info, cfg.size_pixels);
                int unscaled_ascent, unscaled_descent, unscaled_line_gap;
                stbtt_GetFontVMetrics(&src_tmp.font_info, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

                const float ascent = floor(unscaled_ascent * font_scale + ((unscaled_ascent > 0.0f) ? +1 : -1));
                const float descent = floor(unscaled_descent * font_scale + ((unscaled_descent > 0.0f) ? +1 : -1));
                atlas_build_setup_font(atlas, dst_font, &cfg, ascent, descent);
                const float font_off_x = cfg.glyph_offset.x;
                const float font_off_y = cfg.glyph_offset.y + round(dst_font->ascent);

                for (int glyph_i = 0; glyph_i < src_tmp.glyphs_count; glyph_i++) {
                    const int codepoint = src_tmp.glyphs_list[glyph_i];
                    const stbtt_packedchar& pc = src_tmp.packed_chars[glyph_i];
                    stbtt_aligned_quad q;
                    float unused_x = 0.0f, unused_y = 0.0f;
                    stbtt_GetPackedQuad(src_tmp.packed_chars, atlas->tex_width, atlas->tex_height, glyph_i, &unused_x, &unused_y, &q, 0);
                    dst_font->add_glyph(&cfg, (unsigned short)codepoint, q.x0 + font_off_x, q.y0 + font_off_y, q.x1 + font_off_x, q.y1 + font_off_y, q.s0, q.t0, q.s1, q.t1, pc.xadvance);
                }
            }

            for (int src_i = 0; src_i < src_tmp_array.size(); src_i++)
                src_tmp_array[src_i].~src_data();

            atlas_build_finish(atlas);
            return true;
        }

        void atlas_build_init(atlas* atlas) {
            if (atlas->pack_id_mouse_cursors < 0)
                atlas->pack_id_mouse_cursors = atlas->add_custom_rect_regular(2, 2);

            if (atlas->pack_id_lines < 0)
                atlas->pack_id_lines = atlas->add_custom_rect_regular(65, 64);
        }

        void atlas_build_setup_font(atlas* atlas, font* font, font_config* font_config, float ascent, float descent) {
            if (!font_config->merge_mode) {
                font->clear_output_data();
                font->font_size = font_config->size_pixels;
                font->config_data = font_config;
                font->config_data_count = 0;
                font->container_atlas = atlas;
                font->ascent = ascent;
                font->descent = descent;
            }
            font->config_data_count++;
        }

        void atlas_build_pack_custom_rects(atlas* atlas, void* stbrp_context_opaque) {
            stbrp_context* pack_context = (stbrp_context*)stbrp_context_opaque;
            assert(pack_context != NULL);

            std::vector<atlas_custom_rect>& user_rects = atlas->custom_rects;
            assert(user_rects.size() >= 1);

            std::vector<stbrp_rect> pack_rects;
            pack_rects.resize(user_rects.size());
            memset(pack_rects.data(), 0, (size_t)(pack_rects.size() * (int)sizeof(stbrp_rect)));
            for (int i = 0; i < user_rects.size(); i++) {
                pack_rects[i].w = user_rects[i].width;
                pack_rects[i].h = user_rects[i].height;
            }

            stbrp_pack_rects(pack_context, &pack_rects[0], pack_rects.size());
            for (int i = 0; i < pack_rects.size(); i++)
                if (pack_rects[i].was_packed) {
                    user_rects[i].x = pack_rects[i].x;
                    user_rects[i].y = pack_rects[i].y;
                    assert(pack_rects[i].w == user_rects[i].width && pack_rects[i].h == user_rects[i].height);
                    atlas->tex_height = math::max(atlas->tex_height, pack_rects[i].y + pack_rects[i].h);
                }
        }

        void atlas_build_finish(atlas* atlas) {
            assert(atlas->tex_pixels_alpha8 != NULL);
            atlas_build_render_default_tex_data(atlas);
            atlas_build_render_lines_tex_data(atlas);

            for (int i = 0; i < atlas->custom_rects.size(); i++) {
                const atlas_custom_rect* r = &atlas->custom_rects[i];
                if (r->font == NULL || r->glyph_id == 0)
                    continue;

                assert(r->font->container_atlas == atlas);
                vec2 uv0, uv1;
                atlas->calc_custom_rect_uv(r, &uv0, &uv1);
                r->font->add_glyph(NULL, (unsigned short)r->glyph_id, r->glyph_offset.x, r->glyph_offset.y, r->glyph_offset.x + r->width, r->glyph_offset.y + r->height, uv0.x, uv0.y, uv1.x, uv1.y, r->glyph_advance_x);
            }

            for (int i = 0; i < atlas->fonts.size(); i++)
                if (atlas->fonts[i]->dirty_lookup_tables)
                    atlas->fonts[i]->build_lookup_table();

            for (int i = 0; i < atlas->fonts.size(); i++) {
                font* font = atlas->fonts[i];
                if (font->ellipsis_char != (unsigned short)-1)
                    continue;
                const unsigned short ellipsis_variants[] = { (unsigned short)0x2026, (unsigned short)0x0085 };
                for (int j = 0; j < ARRAYSIZE(ellipsis_variants); j++)
                    if (font->find_glyph_no_fallback(ellipsis_variants[j]) != NULL) {
                        font->ellipsis_char = ellipsis_variants[j];
                        break;
                    }
            }
        }

        void atlas_build_render_1bpp_rect_from_string(atlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned char in_marker_pixel_value) {
            assert(x >= 0 && x + w <= atlas->tex_width);
            assert(y >= 0 && y + h <= atlas->tex_height);
            unsigned char* out_pixel = atlas->tex_pixels_alpha8 + x + (y * atlas->tex_width);
            for (int off_y = 0; off_y < h; off_y++, out_pixel += atlas->tex_width, in_str += w)
                for (int off_x = 0; off_x < w; off_x++)
                    out_pixel[off_x] = (in_str[off_x] == in_marker_char) ? in_marker_pixel_value : 0x00;
        }

        void atlas_build_multiply_calc_lookup_table(unsigned char out_table[256], float in_brighten_factor) {
            for (unsigned int i = 0; i < 256; i++) {
                unsigned int value = (unsigned int)(i * in_brighten_factor);
                out_table[i] = value > 255 ? 255 : (value & 0xFF);
            }
        }

        void atlas_build_multiply_rect_alpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride) {
            unsigned char* data = pixels + x + y * stride;
            for (int j = h; j > 0; j--, data += stride)
                for (int i = 0; i < w; i++)
                    data[i] = table[data[i]];
        }

        std::string convert_utf8(std::string text) {
            int size = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS | MB_PRECOMPOSED | MB_USEGLYPHCHARS, text.data(),
                text.length(), nullptr, 0);
            std::wstring utf16_str(size, '\0');
            MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS | MB_PRECOMPOSED | MB_USEGLYPHCHARS, text.data(),
                text.length(), &utf16_str[0], size);

            int utf8_size = WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK | WC_ERR_INVALID_CHARS | WC_NO_BEST_FIT_CHARS, utf16_str.data(),
                utf16_str.length(), nullptr, 0,
                nullptr, nullptr);
            std::string utf8_str(utf8_size, '\0');
            WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK | WC_ERR_INVALID_CHARS | WC_NO_BEST_FIT_CHARS, utf16_str.data(),
                utf16_str.length(), &utf8_str[0], utf8_size,
                nullptr, nullptr);

            return utf8_str;
        }

        std::string erase_utf8(std::string text) {
            const char* utf8 = text.c_str();
            int length = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8, -1, NULL, 0);
            if (length > 0) {
                wchar_t* wide = new wchar_t[length];
                MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8, -1, wide, length);
                size_t convertedChars = 0;
                char* ansi = new char[length];
                wcstombs_s(&convertedChars, ansi, length, wide, _TRUNCATE);

                return std::string(ansi);

            }
            return text;
        }
    }

    font::font() {
        font_size = 0.0f;
        fallback_advance_x = 0.0f;
        fallback_char = (unsigned short)'?';
        ellipsis_char = (unsigned short)-1;
        fallback_glyph = NULL;
        container_atlas = NULL;
        config_data = NULL;
        config_data_count = 0;
        dirty_lookup_tables = false;
        scale = 1.0f;
        ascent = descent = 0.0f;
        memset(used_4k_pages_map, 0, sizeof(used_4k_pages_map));
    }

    font::~font() {
        clear_output_data();
    }

    helpers::glyph* font::find_glyph(unsigned short c) {
        if (c >= (size_t)index_lookup.size()) return fallback_glyph;
        const unsigned short i = index_lookup.data()[c];
        if (i == (unsigned short)-1) return fallback_glyph;
        return &glyphs.data()[i];
    }

    helpers::glyph* font::find_glyph_no_fallback(unsigned short c) {
        if (c >= (size_t)index_lookup.size()) return NULL;
        const unsigned short i = index_lookup.data()[c];
        if (i == (unsigned short)-1) return NULL;
        return &glyphs.data()[i];
    }

    vec2 font::calc_text_size(std::string text, float size) const {
        text = helpers::convert_utf8(text);
        size = size < 0.f ? font_size : size;
        const char* text_end = text.c_str() + strlen(text.c_str());

        const float line_height = size;
        const float scale = size / font_size;

        vec2 text_size = vec2(0, 0);
        float line_width = 0.0f;

        const char* s = text.c_str();
        while (s < text_end) {
            const char* prev_s = s;
            unsigned int c = (unsigned int)*s;
            if (c < 0x80) {
                s += 1;
            } else {
                s += helpers::get_char_from_utf8(&c, s, text_end);
                if (c == 0)
                    break;
            }

            if (c < 32) {
                if (c == '\n') {
                    text_size.x = math::max(text_size.x, line_width);
                    text_size.y += line_height;
                    line_width = 0.0f;
                    continue;
                }
                if (c == '\r') continue;
            }

            const float char_width = ((int)c < index_advance_x.size() ? index_advance_x.data()[c] : fallback_advance_x) * scale;

            line_width += char_width;
        }

        if (text_size.x < line_width) text_size.x = line_width;
        if (line_width > 0 || text_size.y == 0.0f) text_size.y += line_height;

        return text_size;
    }

    const char* font::calc_word_wrap_position_a(float scale, const char* text, const char* text_end, float wrap_width) const {
        float line_width = 0.0f;
        float word_width = 0.0f;
        float blank_width = 0.0f;
        wrap_width /= scale;

        const char* word_end = text;
        const char* prev_word_end = NULL;
        bool inside_word = true;

        const char* s = text;
        while (s < text_end) {
            unsigned int c = (unsigned int)*s;
            const char* next_s;
            if (c < 0x80) next_s = s + 1;
            else next_s = s + helpers::get_char_from_utf8(&c, s, text_end);
            if (c == 0)
                break;

            if (c < 32) {
                if (c == '\n') {
                    line_width = word_width = blank_width = 0.0f;
                    inside_word = true;
                    s = next_s;
                    continue;
                }
                
                if (c == '\r') {
                    s = next_s;
                    continue;
                }
            }

            const float char_width = ((int)c < index_advance_x.size() ? index_advance_x.data()[c] : fallback_advance_x);
            if (c == ' ' || c == '\t' || c == 0x3000) {
                if (inside_word) {
                    line_width += blank_width;
                    blank_width = 0.0f;
                    word_end = s;
                }
                blank_width += char_width;
                inside_word = false;
            } else {
                word_width += char_width;
                if (inside_word) {
                    word_end = next_s;
                } else {
                    prev_word_end = word_end;
                    line_width += word_width + blank_width;
                    word_width = blank_width = 0.0f;
                }

                inside_word = (c != '.' && c != ',' && c != ';' && c != '!' && c != '?' && c != '\"');
            }

            if (line_width + word_width > wrap_width) {
                if (word_width < wrap_width)
                    s = prev_word_end ? prev_word_end : word_end;
                break;
            }

            s = next_s;
        }

        return s;
    }

    void font::build_lookup_table() {
        int max_codepoint = 0;
        for (int i = 0; i != glyphs.size(); i++)
            max_codepoint = math::max(max_codepoint, (int)glyphs[i].codepoint);

        assert(glyphs.size() < 0xFFFF);
        index_advance_x.clear();
        index_lookup.clear();
        dirty_lookup_tables = false;
        memset(used_4k_pages_map, 0, sizeof(used_4k_pages_map));
        grow_index(max_codepoint + 1);
        for (int i = 0; i < glyphs.size(); i++) {
            int codepoint = (int)glyphs[i].codepoint;
            index_advance_x[codepoint] = glyphs[i].advance_x;
            index_lookup[codepoint] = (unsigned short)i;

            const int page_n = codepoint / 4096;
            used_4k_pages_map[page_n >> 3] |= 1 << (page_n & 7);
        }

        if (find_glyph((unsigned short)' ')) {
            if (glyphs.back().codepoint != '\t')
                glyphs.resize(glyphs.size() + 1);
            helpers::glyph& tab_glyph = glyphs.back();
            tab_glyph = *find_glyph((unsigned short)' ');
            tab_glyph.codepoint = '\t';
            tab_glyph.advance_x *= 4;
            index_advance_x[(int)tab_glyph.codepoint] = (float)tab_glyph.advance_x;
            index_lookup[(int)tab_glyph.codepoint] = (unsigned short)(glyphs.size() - 1);
        }

        set_glyph_visible((unsigned short)' ', false);
        set_glyph_visible((unsigned short)'\t', false);

        fallback_glyph = find_glyph_no_fallback(fallback_char);
        fallback_advance_x = fallback_glyph ? fallback_glyph->advance_x : 0.0f;
        for (int i = 0; i < max_codepoint + 1; i++)
            if (index_advance_x[i] < 0.0f)
                index_advance_x[i] = fallback_advance_x;
    }

    void font::clear_output_data() {
        font_size = 0.0f;
        fallback_advance_x = 0.0f;
        glyphs.clear();
        index_advance_x.clear();
        index_lookup.clear();
        fallback_glyph = NULL;
        container_atlas = NULL;
        dirty_lookup_tables = true;
        ascent = descent = 0.0f;
    }

    void font::grow_index(int new_size) {
        assert(index_advance_x.size() == index_lookup.size());
        if (new_size <= index_lookup.size()) return;
        index_advance_x.resize(new_size, -1.0f);
        index_lookup.resize(new_size, (unsigned short)-1);
    }

    void font::add_glyph(helpers::font_config* cfg, unsigned short codepoint, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x) {
        if (cfg != NULL) {
            const float advance_x_original = advance_x;
            advance_x = math::clamp(advance_x, cfg->glyph_min_advance_x, cfg->glyph_max_advance_x);
            if (advance_x != advance_x_original) {
                float char_off_x = cfg->pixel_snap_h ? floor((advance_x - advance_x_original) * 0.5f) : (advance_x - advance_x_original) * 0.5f;
                x0 += char_off_x;
                x1 += char_off_x;
            }

            if (cfg->pixel_snap_h) advance_x = round(advance_x);
            advance_x += cfg->glyph_extra_spacing.x;
        }

        glyphs.resize(glyphs.size() + 1);
        helpers::glyph& glyph = glyphs.back();
        glyph.codepoint = (unsigned int)codepoint;
        glyph.visible = (x0 != x1) && (y0 != y1);
        glyph.x0 = x0;
        glyph.y0 = y0;
        glyph.x1 = x1;
        glyph.y1 = y1;
        glyph.u0 = u0;
        glyph.v0 = v0;
        glyph.u1 = u1;
        glyph.v1 = v1;
        glyph.advance_x = advance_x;
        float pad = container_atlas->tex_glyph_padding + 0.99f;
        dirty_lookup_tables = true;
    }

    void font::add_remap_char(unsigned short dst, unsigned short src, bool overwrite_dst) {
        assert(index_lookup.size() > 0);
        unsigned int index_size = (unsigned int)index_lookup.size();

        if (dst < index_size && index_lookup.data()[dst] == (unsigned short)-1 && !overwrite_dst) return;
        if (src >= index_size && dst >= index_size) return;

        grow_index(dst + 1);
        index_lookup[dst] = (src < index_size) ? index_lookup.data()[src] : (unsigned short)-1;
        index_advance_x[dst] = (src < index_size) ? index_advance_x.data()[src] : 1.0f;
    }

    void font::set_glyph_visible(unsigned short c, bool visible) {
        if (helpers::glyph* glyph = (helpers::glyph*)(void*)find_glyph((unsigned short)c))
            glyph->visible = visible ? 1 : 0;
    }

    void font::set_fallback_char(unsigned short c) {
        fallback_char = c;
        build_lookup_table();
    }

    bool font::is_glyph_range_unused(unsigned int c_begin, unsigned int c_last) {
        unsigned int page_begin = (c_begin / 4096);
        unsigned int page_last = (c_last / 4096);
        for (unsigned int page_n = page_begin; page_n <= page_last; page_n++)
            if ((page_n >> 3) < sizeof(used_4k_pages_map))
                if (used_4k_pages_map[page_n >> 3] & (1 << (page_n & 7)))
                    return false;
        return true;
    }

    void set_current_font(font* font) {
        assert(font && font->is_loaded());
        assert(font->scale > 0.0f);
        vars::main_font = font;
        vars::font_base_size = math::max(1.0f, vars::font_global_scale * vars::main_font->font_size * vars::main_font->scale);
        vars::font_size = font->font_size;

        helpers::atlas* atlas = vars::main_font->container_atlas;
        null_render::shared_data.tex_uv_white_pixel = atlas->tex_uv_white_pixel;
        null_render::shared_data.tex_uv_lines = atlas->tex_uv_lines;
        null_render::shared_data.font = vars::main_font;
        null_render::shared_data.font_size = vars::font_size;
    }

    vec2 text_size(std::string text) {
        font* font = vars::main_font;
        vec2 text_size = font->calc_text_size(text);
        text_size.x = floor(text_size.x + 0.95f);

        return text_size;
    }

    void push_font(font* font) {
        if (!font) font = get_default_font();
        set_current_font(font);
        vars::font_list.push_back(font);
    }

    void pop_font() {
        vars::font_list.pop_back();
        set_current_font(vars::font_list.empty() ? get_default_font() : vars::font_list.back());
    }
}

namespace null_render {
    namespace helpers {
        void draw_data::de_index_all_buffers() {
            std::vector<vertex> new_vtx_buffer;
            total_vtx_count = total_idx_count = 0;
            for (int i = 0; i < cmd_lists_count; i++) {
                draw_list* cmd_list = cmd_lists[i];
                if (cmd_list->idx_buffer.empty())
                    continue;
                new_vtx_buffer.resize(cmd_list->idx_buffer.size());
                for (int j = 0; j < cmd_list->idx_buffer.size(); j++)
                    new_vtx_buffer[j] = cmd_list->vtx_buffer[cmd_list->idx_buffer[j]];
                cmd_list->vtx_buffer.swap(new_vtx_buffer);
                cmd_list->idx_buffer.resize(0);
                total_vtx_count += cmd_list->vtx_buffer.size();
            }
        }

        void draw_data::scale_clip_rects(vec2 fb_scale) {
            for (int i = 0; i < cmd_lists_count; i++) {
                draw_list* cmd_list = cmd_lists[i];
                for (int cmd_i = 0; cmd_i < cmd_list->cmd_buffer.size(); cmd_i++) {
                    cmd* cmd = &cmd_list->cmd_buffer[cmd_i];
                    cmd->clip_rect = rect(cmd->clip_rect.min * fb_scale, cmd->clip_rect.max * fb_scale);
                }
            }
        }

        shared_data::shared_data() {
            memset(this, 0, sizeof(*this));
            for (int i = 0; i < ARRAYSIZE(arc_fast_vtx); i++) {
                const float a = ((float)i * 2 * pi) / (float)ARRAYSIZE(arc_fast_vtx);
                arc_fast_vtx[i] = vec2(cosf(a), sinf(a));
            }
        }

        void shared_data::set_circle_segment_max_error(float max_error) {
            if (circle_segment_max_error == max_error) return;
            circle_segment_max_error = max_error;
            for (int i = 0; i < ARRAYSIZE(circle_segment_counts); i++) {
                const float radius = i + 1.0f;
                const int segment_count = math::clamp((int)((pi * 2.0f) / acosf((radius - circle_segment_max_error) / radius)), 12, 512);
                circle_segment_counts[i] = (unsigned char)math::min(segment_count, 255);
            }
        }

        void shade_verts_linear_uv(draw_list* draw_list, int vert_start_idx, int vert_end_idx, vec2 a, vec2 b, vec2 uv_a, vec2 uv_b, bool clamp) {
            const vec2 size = b - a;
            const vec2 uv_size = uv_b - uv_a;
            const vec2 scale = vec2(
                size.x != 0.0f ? (uv_size.x / size.x) : 0.0f,
                size.y != 0.0f ? (uv_size.y / size.y) : 0.0f);

            vertex* vert_start = draw_list->vtx_buffer.data() + vert_start_idx;
            vertex* vert_end = draw_list->vtx_buffer.data() + vert_end_idx;
            if (clamp) {
                const vec2 min = math::min(uv_a, uv_b);
                const vec2 max = math::max(uv_a, uv_b);
                for (vertex* vertex = vert_start; vertex < vert_end; ++vertex) {
                    vec2 asd = vec2(0, 0);
                    vertex->uv = math::clamp(uv_a + math::mul(vec2(vertex->pos.x, vertex->pos.y) - a, scale), min, max);
                }
            } else {
                for (vertex* vertex = vert_start; vertex < vert_end; ++vertex)
                    vertex->uv = uv_a + math::mul(vec2(vertex->pos.x, vertex->pos.y) - a, scale);
            }
        }

        void add_draw_list_to_draw_data(std::vector<draw_list*>* out_list, draw_list* draw_list) {
            draw_list->_pop_unused_draw_cmd();
            if (draw_list->cmd_buffer.size() == 0)
                return;

            assert(draw_list->vtx_buffer.size() == 0 || draw_list->_vtx_write_ptr == draw_list->vtx_buffer.data() + draw_list->vtx_buffer.size());
            assert(draw_list->idx_buffer.size() == 0 || draw_list->_idx_write_ptr == draw_list->idx_buffer.data() + draw_list->idx_buffer.size());
            if (!(draw_list->flags.contains(draw_list_flags::allow_vtx_offset)))
                assert((int)draw_list->_vtx_current_idx == draw_list->vtx_buffer.size());

            if (sizeof(unsigned short) == 2)
                assert(draw_list->_vtx_current_idx < (1 << 16) && "Too many vertices in draw_list using 16-bit indices. Read comment above");

            out_list->push_back(draw_list);
        }

        void setup_draw_data(std::vector<draw_list*>* draw_lists, helpers::draw_data* draw_data) {
            draw_data->valid = true;
            draw_data->cmd_lists = (draw_lists->size() > 0) ? draw_lists->data() : NULL;
            draw_data->cmd_lists_count = draw_lists->size();
            draw_data->total_vtx_count = draw_data->total_idx_count = 0;
            draw_data->display_pos = vec2(0.0f, 0.0f);
            draw_data->display_size = settings::display_size;
            for (int n = 0; n < draw_lists->size(); n++) {
                draw_data->total_vtx_count += draw_lists->data()[n]->vtx_buffer.size();
                draw_data->total_idx_count += draw_lists->data()[n]->idx_buffer.size();
            }
        }
    }

    void draw_list::push_clip_rect(vec2 cr_min, vec2 cr_max, bool intersect_with_current_clip_rect) {
        rect cr(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
        if (intersect_with_current_clip_rect) {
            rect current = _cmd_header.clip_rect;
            if (cr.min.x < current.min.x) cr.min.x = current.min.x;
            if (cr.min.y < current.min.y) cr.min.y = current.min.y;
            if (cr.max.x > current.max.x) cr.max.x = current.max.x;
            if (cr.max.y > current.max.y) cr.max.y = current.max.y;
        }
        cr.max.x = math::max(cr.min.x, cr.max.x);
        cr.max.y = math::max(cr.min.y, cr.max.y);

        _clip_rect_stack.push_back(cr);
        _cmd_header.clip_rect = cr;
        _on_changed_clip_rect();
    }

    void draw_list::push_clip_rect_full_screen() {
        push_clip_rect(_data->clip_rect_fullscreen.min, _data->clip_rect_fullscreen.max);
    }

    void draw_list::pop_clip_rect() {
        _clip_rect_stack.pop_back();
        _cmd_header.clip_rect = (_clip_rect_stack.size() == 0) ? _data->clip_rect_fullscreen : _clip_rect_stack.data()[_clip_rect_stack.size() - 1];
        _on_changed_clip_rect();
    }

    void draw_list::push_texture_id(void* texture_id) {
        _texture_id_stack.push_back(texture_id);
        _cmd_header.texture_id = texture_id;
        _on_changed_texture_id();
    }

    void draw_list::pop_texture_id() {
        _texture_id_stack.pop_back();
        _cmd_header.texture_id = (_texture_id_stack.size() == 0) ? (void*)NULL : _texture_id_stack.data()[_texture_id_stack.size() - 1];
        _on_changed_texture_id();
    }

    void draw_list::draw_line(vec2 start_point, vec2 end_point, color clr, float thickness) {
        if (clr.a() == 0.f) return;
        
        path_line_to(start_point + vec2(0.5f, 0.5f));
        path_line_to(end_point + vec2(0.5f, 0.5f));
        path_stroke(clr, false, thickness);
    }

    void draw_list::draw_rect(vec2 min, vec2 max, color clr, float rounding, flags_list<corner_flags> rounding_corners, float thickness) {
        if (clr.a() == 0.f) return;
        
        if (flags.contains(draw_list_flags::anti_aliased_lines)) path_rect(min + vec2(0.50f, 0.50f), max - vec2(0.50f, 0.50f), rounding, rounding_corners);
        else path_rect(min + vec2(0.50f, 0.50f), max - vec2(0.49f, 0.49f), rounding, rounding_corners);
        path_stroke(clr, true, thickness);
    }

    void draw_list::draw_rect_filled(vec2 min, vec2 max, color clr, float rounding, flags_list<corner_flags> rounding_corners) {
        if (clr.a() == 0.f) return;
        
        if (rounding > 0.0f) {
            path_rect(min, max, rounding, rounding_corners);
            path_fill_convex(clr);
        } else {
            prim_reserve(6, 4);
            prim_rect(min, max, clr);
        }
    }

    void draw_list::draw_rect_filled_multicolor(vec2 min, vec2 max, std::array<color, 2> upper_colors, std::array<color, 2> bottom_colors, float rounding, flags_list<corner_flags> rounding_corners) {
        if (upper_colors[0].a() == 0.f && upper_colors[1].a() == 0.f && bottom_colors[0].a() == 0.f && bottom_colors[1].a() == 0.f) return;

        if (rounding > 0.0f) {
            path_rect(min, max, rounding, rounding_corners);
            path_fill_convex_multicolor(min, vec2(max.x, min.y), upper_colors, bottom_colors);
        } else {
            const vec2 uv = _data->tex_uv_white_pixel;
            prim_reserve(6, 4);
            prim_write_idx((unsigned short)(_vtx_current_idx)); prim_write_idx((unsigned short)(_vtx_current_idx + 1)); prim_write_idx((unsigned short)(_vtx_current_idx + 2));
            prim_write_idx((unsigned short)(_vtx_current_idx)); prim_write_idx((unsigned short)(_vtx_current_idx + 2)); prim_write_idx((unsigned short)(_vtx_current_idx + 3));
            prim_write_vtx(min, uv, upper_colors[0]);
            prim_write_vtx(vec2(max.x, min.y), uv, upper_colors[1]);
            prim_write_vtx(max, uv, bottom_colors[1]);
            prim_write_vtx(vec2(min.x, max.y), uv, bottom_colors[0]);
        }
    }

    void draw_list::draw_rect_filled_multicolor_vertical(vec2 min, vec2 max, color upper_color, color bottom_color, float rounding, flags_list<corner_flags> rounding_corners) {
        if (upper_color.a() == 0.f && bottom_color.a() == 0.f) return;

        if (rounding > 0.0f) {
            path_rect(min, max, rounding, rounding_corners);
            path_fill_convex_multicolor_liner(min, vec2(min.x, max.y), upper_color, bottom_color );
        } else {
            draw_rect_filled_multicolor(min, max, { upper_color, upper_color }, { bottom_color, bottom_color });
        }
    }

    void draw_list::draw_rect_filled_multicolor_horizontal(vec2 min, vec2 max, color left_color, color right_color, float rounding, flags_list<corner_flags> rounding_corners) {
        if (left_color.a() == 0.f && right_color.a() == 0.f) return;

        if (rounding > 0.0f) {
            path_rect(min, max, rounding, rounding_corners);
            path_fill_convex_multicolor_liner(min, vec2(max.x, min.y), left_color, right_color );
        } else {
            draw_rect_filled_multicolor(min, max, { left_color, right_color }, { left_color, right_color });
        }
    }

    void draw_list::draw_quad(std::array<vec2, 4> points, color clr, float thickness) {
        if (clr.a() == 0.f) return;

        path_line_to(points[0]);
        path_line_to(points[0]);
        path_line_to(points[0]);
        path_line_to(points[0]);
        path_stroke(clr, true, thickness);
    }

    void draw_list::draw_quad_filled(std::array<vec2, 4> points, color clr) {
        if (clr.a() == 0.f) return;

        path_line_to(points[0]);
        path_line_to(points[1]);
        path_line_to(points[2]);
        path_line_to(points[3]);
        path_fill_convex(clr);
    }

    void draw_list::draw_triangle(std::array<vec2, 3> points, color clr, float thickness) {
        if (clr.a() == 0.f) return;

        path_line_to(points[0]);
        path_line_to(points[1]);
        path_line_to(points[2]);
        path_stroke(clr, true, thickness);
    }

    void draw_list::draw_triangle_filled(std::array<vec2, 3> points, color clr) {
        if (clr.a() == 0.f) return;

        path_line_to(points[0]);
        path_line_to(points[1]);
        path_line_to(points[2]);
        path_fill_convex(clr);
    }

    void draw_list::draw_circle(vec2 center, color clr, float radius, int num_segments, float thickness) {
        if (clr.a() == 0.f || radius <= 0.0f) return;

        if (num_segments <= 0) {
            const int radius_idx = (int)radius - 1;
            if (radius_idx < ARRAYSIZE(_data->circle_segment_counts)) num_segments = _data->circle_segment_counts[radius_idx];
            else num_segments = math::clamp((int)((pi * 2.0f) / acosf((radius - _data->circle_segment_max_error) / radius)), 12, 512);
        } else {
            num_segments = math::clamp(num_segments, 3, 512);
        }

        const float a_max = (pi * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
        if (num_segments == 12) path_arc_to_fast(center, radius - 0.5f, 0, 12 - 1);
        else path_arc_to(center, radius - 0.5f, 0.0f, a_max, num_segments - 1);
        path_stroke(clr, true, thickness);
    }

    void draw_list::draw_circle_filled(vec2 center, color clr, float radius, int num_segments) {
        if (clr.a() == 0.f || radius <= 0.0f) return;

        if (num_segments <= 0) {
            const int radius_idx = (int)radius - 1;
            if (radius_idx < ARRAYSIZE(_data->circle_segment_counts)) num_segments = _data->circle_segment_counts[radius_idx];
            else num_segments = math::clamp((int)((pi * 2.0f) / acosf((radius - _data->circle_segment_max_error) / radius)), 12, 512);
        } else {
            num_segments = math::clamp(num_segments, 3, 512);
        }

        const float a_max = (pi * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
        if (num_segments == 12) path_arc_to_fast(center, radius, 0, 12 - 1);
        else path_arc_to(center, radius, 0.0f, a_max, num_segments - 1);
        path_fill_convex(clr);
    }

    void draw_list::draw_ngon(vec2 center, float radius, color clr, int num_segments, float thickness) {
        if (clr.a() == 0.f || num_segments <= 2) return;

        const float a_max = (pi * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
        path_arc_to(center, radius - 0.5f, 0.0f, a_max, num_segments - 1);
        path_stroke(clr, true, thickness);
    }

    void draw_list::draw_ngon_filled(vec2 center, float radius, color clr, int num_segments) {
        if (clr.a() == 0.f || num_segments <= 2) return;

        const float a_max = (pi * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
        path_arc_to(center, radius, 0.0f, a_max, num_segments - 1);
        path_fill_convex(clr);
    }

    void draw_list::draw_char(null_font::font* font, float size, vec2 pos, color clr, unsigned short c) {
        const null_font::helpers::glyph* glyph = font->find_glyph(c);
        if (!glyph || !glyph->visible) return;
        float scale = (size >= 0.0f) ? (size / font->font_size) : 1.0f;
        pos.x = floor(pos.x);
        pos.y = floor(pos.y);
        prim_reserve(6, 4);
        prim_rect_uv(vec2(pos.x + glyph->x0 * scale, pos.y + glyph->y0 * scale), vec2(pos.x + glyph->x1 * scale, pos.y + glyph->y1 * scale), vec2(glyph->u0, glyph->v0), vec2(glyph->u1, glyph->v1), clr);
    }

    void draw_list::draw_text(std::string text, vec2 pos, color clr, bool outline, std::array<bool, 2> centered, null_font::font* font, float size) {
        if(font == NULL) font = _data->font;
        if(size == 0.f) size = _data->font_size;
        if (centered[0]) pos.x -= font->calc_text_size(text).x / 2;
        if (centered[1]) pos.y -= font->calc_text_size(text).y / 2;

        if (outline) {
            draw_text(text, pos + vec2(1.f, 0.f), color(0.f, 0.f, 0.f, clr.a()), font, size);
            draw_text(text, pos - vec2(1.f, 0.f), color(0.f, 0.f, 0.f, clr.a()), font, size);
            draw_text(text, pos + vec2(0.f, 1.f), color(0.f, 0.f, 0.f, clr.a()), font, size);
            draw_text(text, pos - vec2(0.f, 1.f), color(0.f, 0.f, 0.f, clr.a()), font, size);
        }

        draw_text(text, pos, clr, font, size);
    }

    void draw_list::draw_text(std::string text, vec2 pos, color clr, null_font::font* font, float size, rect* _clip_rect, bool cpu_fine_clip) {
        text = null_font::helpers::convert_utf8(text);

        if (clr.a() == 0.f) return;
        if (font == NULL) font = _data->font;
        if (size == 0.0f) size = _data->font_size;

        assert(font->container_atlas->tex_id == _cmd_header.texture_id);

        rect clip_rect = _cmd_header.clip_rect;
        if (cpu_fine_clip) {
            clip_rect.min.x = math::max(clip_rect.min.x, _clip_rect->min.x);
            clip_rect.min.y = math::max(clip_rect.min.y, _clip_rect->min.y);
            clip_rect.max.x = math::min(clip_rect.max.x, _clip_rect->max.x);
            clip_rect.max.y = math::min(clip_rect.max.y, _clip_rect->max.y);
        }

        const char* text_end = text.c_str() + strlen(text.c_str());

        pos.x = floor(pos.x);
        pos.y = floor(pos.y);
        float x = pos.x;
        float y = pos.y;
        if (y > clip_rect.max.y) return;

        const float scale = size / font->font_size;
        const float line_height = font->font_size * scale;

        const char* s = text.c_str();
        if (y + line_height < clip_rect.min.y)
            while (y + line_height < clip_rect.min.y && s < text_end) {
                s = (const char*)memchr(s, '\n', text_end - s);
                s = s ? s + 1 : text_end;
                y += line_height;
            }

        if (text_end - s > 10000) {
            const char* s_end = s;
            float y_end = y;
            while (y_end < clip_rect.max.y && s_end < text_end) {
                s_end = (const char*)memchr(s_end, '\n', text_end - s_end);
                s_end = s_end ? s_end + 1 : text_end;
                y_end += line_height;
            }
            text_end = s_end;
        }
        if (s == text_end) return;

        int vtx_count_max = (int)(text_end - s) * 4;
        int idx_count_max = (int)(text_end - s) * 6;
        const int idx_expected_size = idx_buffer.size() + idx_count_max;
        prim_reserve(idx_count_max, vtx_count_max);

        helpers::vertex* vtx_write = _vtx_write_ptr;
        unsigned short* idx_write = _idx_write_ptr;
        unsigned int vtx_current_idx = _vtx_current_idx;

        while (s < text_end) {
            unsigned int c = (unsigned int)*s;
            if (c < 0x80) {
                s += 1;
            } else {
                s += null_font::helpers::get_char_from_utf8(&c, s, text_end);
                if (c == 0)
                    break;
            }

            if (c < 32) {
                if (c == '\n') {
                    x = pos.x;
                    y += line_height;
                    if (y > clip_rect.max.y)
                        break;
                    continue;
                }
                if (c == '\r')
                    continue;
            }

            const null_font::helpers::glyph* glyph = font->find_glyph((unsigned short)c);
            if (glyph == NULL)
                continue;

            float char_width = glyph->advance_x * scale;
            if (glyph->visible) {
                float x1 = x + glyph->x0 * scale;
                float x2 = x + glyph->x1 * scale;
                float y1 = y + glyph->y0 * scale;
                float y2 = y + glyph->y1 * scale;
                if (x1 <= clip_rect.max.x && x2 >= clip_rect.min.x) {
                    float u1 = glyph->u0;
                    float v1 = glyph->v0;
                    float u2 = glyph->u1;
                    float v2 = glyph->v1;

                    if (cpu_fine_clip) {
                        if (x1 < clip_rect.min.x) {
                            u1 = u1 + (1.0f - (x2 - clip_rect.min.x) / (x2 - x1)) * (u2 - u1);
                            x1 = clip_rect.min.x;
                        }

                        if (y1 < clip_rect.min.y) {
                            v1 = v1 + (1.0f - (y2 - clip_rect.min.y) / (y2 - y1)) * (v2 - v1);
                            y1 = clip_rect.min.y;
                        }

                        if (x2 > clip_rect.max.x) {
                            u2 = u1 + ((clip_rect.max.x - x1) / (x2 - x1)) * (u2 - u1);
                            x2 = clip_rect.max.x;
                        }

                        if (y2 > clip_rect.max.y) {
                            v2 = v1 + ((clip_rect.max.y - y1) / (y2 - y1)) * (v2 - v1);
                            y2 = clip_rect.max.y;
                        }

                        if (y1 >= y2) {
                            x += char_width;
                            continue;
                        }
                    }

                    {
                        idx_write[0] = (unsigned short)(vtx_current_idx); idx_write[1] = (unsigned short)(vtx_current_idx + 1); idx_write[2] = (unsigned short)(vtx_current_idx + 2);
                        idx_write[3] = (unsigned short)(vtx_current_idx); idx_write[4] = (unsigned short)(vtx_current_idx + 2); idx_write[5] = (unsigned short)(vtx_current_idx + 3);
                        vtx_write[0].pos.x = x1; vtx_write[0].pos.y = y1; vtx_write[0].clr = clr; vtx_write[0].uv.x = u1; vtx_write[0].uv.y = v1;
                        vtx_write[1].pos.x = x2; vtx_write[1].pos.y = y1; vtx_write[1].clr = clr; vtx_write[1].uv.x = u2; vtx_write[1].uv.y = v1;
                        vtx_write[2].pos.x = x2; vtx_write[2].pos.y = y2; vtx_write[2].clr = clr; vtx_write[2].uv.x = u2; vtx_write[2].uv.y = v2;
                        vtx_write[3].pos.x = x1; vtx_write[3].pos.y = y2; vtx_write[3].clr = clr; vtx_write[3].uv.x = u1; vtx_write[3].uv.y = v2;
                        vtx_write += 4;
                        vtx_current_idx += 4;
                        idx_write += 6;
                    }
                }
            }
            x += char_width;
        }

        vtx_buffer.resize((int)(vtx_write - vtx_buffer.data()));
        idx_buffer.resize((int)(idx_write - idx_buffer.data()));
        cmd_buffer[cmd_buffer.size() - 1].elem_count -= (idx_expected_size - idx_buffer.size());
        _vtx_write_ptr = vtx_write;
        _idx_write_ptr = idx_write;
        _vtx_current_idx = vtx_current_idx;
    }

    void draw_list::draw_poly_line(std::vector<vec2> points, color clr, bool closed, float thickness) {
        if (points.size() < 2) return;

        const vec2 opaque_uv = _data->tex_uv_white_pixel;
        const int count = closed ? points.size() : points.size() - 1;
        const bool thick_line = (thickness > 1.0f);

        if (flags.contains(draw_list_flags::anti_aliased_lines)) {
            const float AA_SIZE = 1.0f;

            thickness = math::max(thickness, 1.0f);
            const int integer_thickness = (int)thickness;
            const float fractional_thickness = thickness - integer_thickness;

            const bool use_texture = (flags.contains(draw_list_flags::anti_aliased_lines_use_tex)) && (integer_thickness < 63) && (fractional_thickness <= 0.00001f);

            const int idx_count = use_texture ? (count * 6) : (thick_line ? count * 18 : count * 12);
            const int vtx_count = use_texture ? (points.size() * 2) : (thick_line ? points.size() * 4 : points.size() * 3);
            prim_reserve(idx_count, vtx_count);

            vec2* temp_normals = (vec2*)alloca(points.size() * ((use_texture || !thick_line) ? 3 : 5) * sizeof(vec2));
            vec2* temp_points = temp_normals + points.size();

            for (int i1 = 0; i1 < count; i1++) {
                const int i2 = (i1 + 1) == points.size() ? 0 : i1 + 1;
                float dx = points[i2].x - points[i1].x;
                float dy = points[i2].y - points[i1].y;
                NORMALIZE2F_OVER_ZERO(dx, dy);
                temp_normals[i1].x = dy;
                temp_normals[i1].y = -dx;
            }
            
            if (!closed)
                temp_normals[points.size() - 1] = temp_normals[points.size() - 2];

            if (use_texture || !thick_line) {
                const float half_draw_size = use_texture ? ((thickness * 0.5f) + 1) : AA_SIZE;
                if (!closed) {
                    temp_points[0] = points[0] + temp_normals[0] * half_draw_size;
                    temp_points[1] = points[0] - temp_normals[0] * half_draw_size;
                    temp_points[(points.size() - 1) * 2 + 0] = points[points.size() - 1] + temp_normals[points.size() - 1] * half_draw_size;
                    temp_points[(points.size() - 1) * 2 + 1] = points[points.size() - 1] - temp_normals[points.size() - 1] * half_draw_size;
                }

                unsigned int idx1 = _vtx_current_idx;
                for (int i1 = 0; i1 < count; i1++) {
                    const int i2 = (i1 + 1) == points.size() ? 0 : i1 + 1;
                    const unsigned int idx2 = ((i1 + 1) == points.size()) ? _vtx_current_idx : (idx1 + (use_texture ? 2 : 3));

                    float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                    float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                    FIXNORMAL2F(dm_x, dm_y);
                    dm_x *= half_draw_size;
                    dm_y *= half_draw_size;

                    vec2* out_vtx = &temp_points[i2 * 2];
                    out_vtx[0].x = points[i2].x + dm_x;
                    out_vtx[0].y = points[i2].y + dm_y;
                    out_vtx[1].x = points[i2].x - dm_x;
                    out_vtx[1].y = points[i2].y - dm_y;

                    if (use_texture) {
                        _idx_write_ptr[0] = (unsigned short)(idx2 + 0); _idx_write_ptr[1] = (unsigned short)(idx1 + 0); _idx_write_ptr[2] = (unsigned short)(idx1 + 1);
                        _idx_write_ptr[3] = (unsigned short)(idx2 + 1); _idx_write_ptr[4] = (unsigned short)(idx1 + 1); _idx_write_ptr[5] = (unsigned short)(idx2 + 0);
                        _idx_write_ptr += 6;
                    } else {
                        _idx_write_ptr[0] = (unsigned short)(idx2 + 0); _idx_write_ptr[1] = (unsigned short)(idx1 + 0); _idx_write_ptr[2] = (unsigned short)(idx1 + 2);
                        _idx_write_ptr[3] = (unsigned short)(idx1 + 2); _idx_write_ptr[4] = (unsigned short)(idx2 + 2); _idx_write_ptr[5] = (unsigned short)(idx2 + 0);
                        _idx_write_ptr[6] = (unsigned short)(idx2 + 1); _idx_write_ptr[7] = (unsigned short)(idx1 + 1); _idx_write_ptr[8] = (unsigned short)(idx1 + 0);
                        _idx_write_ptr[9] = (unsigned short)(idx1 + 0); _idx_write_ptr[10] = (unsigned short)(idx2 + 0); _idx_write_ptr[11] = (unsigned short)(idx2 + 1);
                        _idx_write_ptr += 12;
                    }

                    idx1 = idx2;
                }

                if (use_texture) {
                    rect tex_uvs = _data->tex_uv_lines[integer_thickness];
                    vec2 tex_uv0 = tex_uvs.min;
                    vec2 tex_uv1 = tex_uvs.max;
                    for (int i = 0; i < points.size(); i++) {
                        _vtx_write_ptr[0].pos = temp_points[i * 2 + 0]; _vtx_write_ptr[0].uv = tex_uv0; _vtx_write_ptr[0].clr = clr;
                        _vtx_write_ptr[1].pos = temp_points[i * 2 + 1]; _vtx_write_ptr[1].uv = tex_uv1; _vtx_write_ptr[1].clr = clr;
                        _vtx_write_ptr += 2;
                    }
                } else {
                    for (int i = 0; i < points.size(); i++) {
                        _vtx_write_ptr[0].pos = points[i];              _vtx_write_ptr[0].uv = opaque_uv; _vtx_write_ptr[0].clr = clr;
                        _vtx_write_ptr[1].pos = temp_points[i * 2 + 0]; _vtx_write_ptr[1].uv = opaque_uv; _vtx_write_ptr[1].clr = color(clr, 0.f);
                        _vtx_write_ptr[2].pos = temp_points[i * 2 + 1]; _vtx_write_ptr[2].uv = opaque_uv; _vtx_write_ptr[2].clr = color(clr, 0.f);
                        _vtx_write_ptr += 3;
                    }
                }
            } else {
                const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;

                if (!closed) {
                    const int points_last = points.size() - 1;
                    temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
                    temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
                    temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
                    temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
                    temp_points[points_last * 4 + 0] = points[points_last] + temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
                    temp_points[points_last * 4 + 1] = points[points_last] + temp_normals[points_last] * (half_inner_thickness);
                    temp_points[points_last * 4 + 2] = points[points_last] - temp_normals[points_last] * (half_inner_thickness);
                    temp_points[points_last * 4 + 3] = points[points_last] - temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
                }

                unsigned int idx1 = _vtx_current_idx;
                for (int i1 = 0; i1 < count; i1++) {
                    const int i2 = (i1 + 1) == points.size() ? 0 : (i1 + 1);
                    const unsigned int idx2 = (i1 + 1) == points.size() ? _vtx_current_idx : (idx1 + 4);

                    float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                    float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                    FIXNORMAL2F(dm_x, dm_y);
                    float dm_out_x = dm_x * (half_inner_thickness + AA_SIZE);
                    float dm_out_y = dm_y * (half_inner_thickness + AA_SIZE);
                    float dm_in_x = dm_x * half_inner_thickness;
                    float dm_in_y = dm_y * half_inner_thickness;

                    vec2* out_vtx = &temp_points[i2 * 4];
                    out_vtx[0].x = points[i2].x + dm_out_x;
                    out_vtx[0].y = points[i2].y + dm_out_y;
                    out_vtx[1].x = points[i2].x + dm_in_x;
                    out_vtx[1].y = points[i2].y + dm_in_y;
                    out_vtx[2].x = points[i2].x - dm_in_x;
                    out_vtx[2].y = points[i2].y - dm_in_y;
                    out_vtx[3].x = points[i2].x - dm_out_x;
                    out_vtx[3].y = points[i2].y - dm_out_y;

                    _idx_write_ptr[0] = (unsigned short)(idx2 + 1); _idx_write_ptr[1] = (unsigned short)(idx1 + 1); _idx_write_ptr[2] = (unsigned short)(idx1 + 2);
                    _idx_write_ptr[3] = (unsigned short)(idx1 + 2); _idx_write_ptr[4] = (unsigned short)(idx2 + 2); _idx_write_ptr[5] = (unsigned short)(idx2 + 1);
                    _idx_write_ptr[6] = (unsigned short)(idx2 + 1); _idx_write_ptr[7] = (unsigned short)(idx1 + 1); _idx_write_ptr[8] = (unsigned short)(idx1 + 0);
                    _idx_write_ptr[9] = (unsigned short)(idx1 + 0); _idx_write_ptr[10] = (unsigned short)(idx2 + 0); _idx_write_ptr[11] = (unsigned short)(idx2 + 1);
                    _idx_write_ptr[12] = (unsigned short)(idx2 + 2); _idx_write_ptr[13] = (unsigned short)(idx1 + 2); _idx_write_ptr[14] = (unsigned short)(idx1 + 3);
                    _idx_write_ptr[15] = (unsigned short)(idx1 + 3); _idx_write_ptr[16] = (unsigned short)(idx2 + 3); _idx_write_ptr[17] = (unsigned short)(idx2 + 2);
                    _idx_write_ptr += 18;

                    idx1 = idx2;
                }

                for (int i = 0; i < points.size(); i++) {
                    _vtx_write_ptr[0].pos = temp_points[i * 4 + 0]; _vtx_write_ptr[0].uv = opaque_uv; _vtx_write_ptr[0].clr = color(clr, 0.f);
                    _vtx_write_ptr[1].pos = temp_points[i * 4 + 1]; _vtx_write_ptr[1].uv = opaque_uv; _vtx_write_ptr[1].clr = clr;
                    _vtx_write_ptr[2].pos = temp_points[i * 4 + 2]; _vtx_write_ptr[2].uv = opaque_uv; _vtx_write_ptr[2].clr = clr;
                    _vtx_write_ptr[3].pos = temp_points[i * 4 + 3]; _vtx_write_ptr[3].uv = opaque_uv; _vtx_write_ptr[3].clr = color(clr, 0.f);
                    _vtx_write_ptr += 4;
                }
            }
            _vtx_current_idx += (unsigned short)vtx_count;
        } else {
            const int idx_count = count * 6;
            const int vtx_count = count * 4;
            prim_reserve(idx_count, vtx_count);

            for (int i1 = 0; i1 < count; i1++) {
                const int i2 = (i1 + 1) == points.size() ? 0 : i1 + 1;
                const vec2& p1 = points[i1];
                const vec2& p2 = points[i2];

                float dx = p2.x - p1.x;
                float dy = p2.y - p1.y;
                NORMALIZE2F_OVER_ZERO(dx, dy);
                dx *= (thickness * 0.5f);
                dy *= (thickness * 0.5f);

                _vtx_write_ptr[0].pos.x = p1.x + dy; _vtx_write_ptr[0].pos.y = p1.y - dx; _vtx_write_ptr[0].uv = opaque_uv; _vtx_write_ptr[0].clr = clr;
                _vtx_write_ptr[1].pos.x = p2.x + dy; _vtx_write_ptr[1].pos.y = p2.y - dx; _vtx_write_ptr[1].uv = opaque_uv; _vtx_write_ptr[1].clr = clr;
                _vtx_write_ptr[2].pos.x = p2.x - dy; _vtx_write_ptr[2].pos.y = p2.y + dx; _vtx_write_ptr[2].uv = opaque_uv; _vtx_write_ptr[2].clr = clr;
                _vtx_write_ptr[3].pos.x = p1.x - dy; _vtx_write_ptr[3].pos.y = p1.y + dx; _vtx_write_ptr[3].uv = opaque_uv; _vtx_write_ptr[3].clr = clr;
                _vtx_write_ptr += 4;

                _idx_write_ptr[0] = (unsigned short)(_vtx_current_idx); _idx_write_ptr[1] = (unsigned short)(_vtx_current_idx + 1); _idx_write_ptr[2] = (unsigned short)(_vtx_current_idx + 2);
                _idx_write_ptr[3] = (unsigned short)(_vtx_current_idx); _idx_write_ptr[4] = (unsigned short)(_vtx_current_idx + 2); _idx_write_ptr[5] = (unsigned short)(_vtx_current_idx + 3);
                _idx_write_ptr += 6;
                _vtx_current_idx += 4;
            }
        }
    }

    void draw_list::draw_convex_poly_filled(std::vector<vec2> points, color clr) {
        if (points.size() < 3) return;

        const vec2 uv = _data->tex_uv_white_pixel;

        if (flags.contains(draw_list_flags::anti_aliased_fill)) {
            const float AA_SIZE = 1.0f;
            const int idx_count = (points.size() - 2) * 3 + points.size() * 6;
            const int vtx_count = (points.size() * 2);
            prim_reserve(idx_count, vtx_count);

            unsigned int vtx_inner_idx = _vtx_current_idx;
            unsigned int vtx_outer_idx = _vtx_current_idx + 1;
            for (int i = 2; i < points.size(); i++) {
                _idx_write_ptr[0] = (unsigned short)(vtx_inner_idx); _idx_write_ptr[1] = (unsigned short)(vtx_inner_idx + ((i - 1) << 1)); _idx_write_ptr[2] = (unsigned short)(vtx_inner_idx + (i << 1));
                _idx_write_ptr += 3;
            }

            vec2* temp_normals = (vec2*)alloca(points.size() * sizeof(vec2));
            for (int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                const vec2& p0 = points[i0];
                const vec2& p1 = points[i1];
                float dx = p1.x - p0.x;
                float dy = p1.y - p0.y;
                NORMALIZE2F_OVER_ZERO(dx, dy);
                temp_normals[i0].x = dy;
                temp_normals[i0].y = -dx;
            }

            for (int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                const vec2& n0 = temp_normals[i0];
                const vec2& n1 = temp_normals[i1];
                float dm_x = (n0.x + n1.x) * 0.5f;
                float dm_y = (n0.y + n1.y) * 0.5f;
                FIXNORMAL2F(dm_x, dm_y);
                dm_x *= AA_SIZE * 0.5f;
                dm_y *= AA_SIZE * 0.5f;

                _vtx_write_ptr[0].pos.x = (points[i1].x - dm_x); _vtx_write_ptr[0].pos.y = (points[i1].y - dm_y); _vtx_write_ptr[0].uv = uv; _vtx_write_ptr[0].clr = clr;
                _vtx_write_ptr[1].pos.x = (points[i1].x + dm_x); _vtx_write_ptr[1].pos.y = (points[i1].y + dm_y); _vtx_write_ptr[1].uv = uv; _vtx_write_ptr[1].clr = color(clr, 0.f);
                _vtx_write_ptr += 2;

                _idx_write_ptr[0] = (unsigned short)(vtx_inner_idx + (i1 << 1)); _idx_write_ptr[1] = (unsigned short)(vtx_inner_idx + (i0 << 1)); _idx_write_ptr[2] = (unsigned short)(vtx_outer_idx + (i0 << 1));
                _idx_write_ptr[3] = (unsigned short)(vtx_outer_idx + (i0 << 1)); _idx_write_ptr[4] = (unsigned short)(vtx_outer_idx + (i1 << 1)); _idx_write_ptr[5] = (unsigned short)(vtx_inner_idx + (i1 << 1));
                _idx_write_ptr += 6;
            }
            _vtx_current_idx += (unsigned short)vtx_count;
        } else {
            const int idx_count = (points.size() - 2) * 3;
            const int vtx_count = points.size();
            prim_reserve(idx_count, vtx_count);
            for (int i = 0; i < vtx_count; i++) {
                _vtx_write_ptr[0].pos = points[i]; _vtx_write_ptr[0].uv = uv; _vtx_write_ptr[0].clr = clr;
                _vtx_write_ptr++;
            }

            for (int i = 2; i < points.size(); i++) {
                _idx_write_ptr[0] = (unsigned short)(_vtx_current_idx); _idx_write_ptr[1] = (unsigned short)(_vtx_current_idx + i - 1); _idx_write_ptr[2] = (unsigned short)(_vtx_current_idx + i);
                _idx_write_ptr += 3;
            }
            _vtx_current_idx += (unsigned short)vtx_count;
        }
    }

    void draw_list::draw_convex_poly_filled_multicolor(std::vector<vec2> points, vec2 min, vec2 max, std::array<color, 2> upper_colors, std::array<color, 2> bottom_colors) {
        if (points.size() < 3) return;

        const vec2 uv = _data->tex_uv_white_pixel;

        if (flags.contains(draw_list_flags::anti_aliased_fill)) {
            const float AA_SIZE = 1.0f;
            const int idx_count = (points.size() - 2) * 3 + points.size() * 6;
            const int vtx_count = (points.size() * 2);
            prim_reserve(idx_count, vtx_count);

            unsigned int vtx_inner_idx = _vtx_current_idx;
            unsigned int vtx_outer_idx = _vtx_current_idx + 1;
            for (int i = 2; i < points.size(); i++) {
                _idx_write_ptr[0] = (unsigned short)(vtx_inner_idx); _idx_write_ptr[1] = (unsigned short)(vtx_inner_idx + ((i - 1) << 1)); _idx_write_ptr[2] = (unsigned short)(vtx_inner_idx + (i << 1));
                _idx_write_ptr += 3;
            }

            vec2* temp_normals = (vec2*)alloca(points.size() * sizeof(vec2));
            for (int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                const vec2& p0 = points[i0];
                const vec2& p1 = points[i1];
                float dx = p1.x - p0.x;
                float dy = p1.y - p0.y;
                NORMALIZE2F_OVER_ZERO(dx, dy);
                temp_normals[i0].x = dy;
                temp_normals[i0].y = -dx;
            }

            for (int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                const vec2& n0 = temp_normals[i0];
                const vec2& n1 = temp_normals[i1];
                float dm_x = (n0.x + n1.x) * 0.5f;
                float dm_y = (n0.y + n1.y) * 0.5f;
                FIXNORMAL2F(dm_x, dm_y);
                dm_x *= AA_SIZE * 0.5f;
                dm_y *= AA_SIZE * 0.5f;

                color clr;
                float t = math::clamp((points[i1] - min).dot(max - min) * (1.0f / (max - min).length_sqr()), 0.0f, 1.0f);
                if (i1 < points.size() / 2) clr = color(upper_colors[0] + (upper_colors[1] - upper_colors[0]) * t);
                else clr = color(bottom_colors[0] + (bottom_colors[1] - bottom_colors[0]) * t);

                _vtx_write_ptr[0].pos.x = (points[i1].x - dm_x); _vtx_write_ptr[0].pos.y = (points[i1].y - dm_y); _vtx_write_ptr[0].uv = uv; _vtx_write_ptr[0].clr = clr;
                _vtx_write_ptr[1].pos.x = (points[i1].x + dm_x); _vtx_write_ptr[1].pos.y = (points[i1].y + dm_y); _vtx_write_ptr[1].uv = uv; _vtx_write_ptr[1].clr = color(clr, 0.f);
                _vtx_write_ptr += 2;

                _idx_write_ptr[0] = (unsigned short)(vtx_inner_idx + (i1 << 1)); _idx_write_ptr[1] = (unsigned short)(vtx_inner_idx + (i0 << 1)); _idx_write_ptr[2] = (unsigned short)(vtx_outer_idx + (i0 << 1));
                _idx_write_ptr[3] = (unsigned short)(vtx_outer_idx + (i0 << 1)); _idx_write_ptr[4] = (unsigned short)(vtx_outer_idx + (i1 << 1)); _idx_write_ptr[5] = (unsigned short)(vtx_inner_idx + (i1 << 1));
                _idx_write_ptr += 6;
            }
            _vtx_current_idx += (unsigned short)vtx_count;
        } else {
            const int idx_count = (points.size() - 2) * 3;
            const int vtx_count = points.size();
            prim_reserve(idx_count, vtx_count);
            for (int i = 0; i < vtx_count; i++) {      
                color clr;
                float t = math::clamp((points[i] - min).dot(vec2(max.x, 0.f) - min) * (1.0f / (vec2(max.x, 0.f) - min).length_sqr()), 0.0f, 1.0f);
                if (i < points.size() / 2) clr = color(upper_colors[0] + (upper_colors[1] - upper_colors[0]) * t);
                else clr = color(bottom_colors[0] + (bottom_colors[1] - bottom_colors[0]) * t);

                _vtx_write_ptr[0].pos = points[i]; _vtx_write_ptr[0].uv = uv; _vtx_write_ptr[0].clr = clr;
                _vtx_write_ptr++;
            }

            for (int i = 2; i < points.size(); i++) {
                _idx_write_ptr[0] = (unsigned short)(_vtx_current_idx); _idx_write_ptr[1] = (unsigned short)(_vtx_current_idx + i - 1); _idx_write_ptr[2] = (unsigned short)(_vtx_current_idx + i);
                _idx_write_ptr += 3;
            }
            _vtx_current_idx += (unsigned short)vtx_count;
        }
    }

    void draw_list::draw_convex_poly_filled_multicolor_liner(std::vector<vec2> points, vec2 min, vec2 max, color first_color, color second_color) {
        if (points.size() < 3) return;

        const vec2 uv = _data->tex_uv_white_pixel;

        if (flags.contains(draw_list_flags::anti_aliased_fill)) {
            const float AA_SIZE = 1.0f;
            const int idx_count = (points.size() - 2) * 3 + points.size() * 6;
            const int vtx_count = (points.size() * 2);
            prim_reserve(idx_count, vtx_count);

            unsigned int vtx_inner_idx = _vtx_current_idx;
            unsigned int vtx_outer_idx = _vtx_current_idx + 1;
            for (int i = 2; i < points.size(); i++) {
                _idx_write_ptr[0] = (unsigned short)(vtx_inner_idx); _idx_write_ptr[1] = (unsigned short)(vtx_inner_idx + ((i - 1) << 1)); _idx_write_ptr[2] = (unsigned short)(vtx_inner_idx + (i << 1));
                _idx_write_ptr += 3;
            }

            vec2* temp_normals = (vec2*)alloca(points.size() * sizeof(vec2));
            for (int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                const vec2& p0 = points[i0];
                const vec2& p1 = points[i1];
                float dx = p1.x - p0.x;
                float dy = p1.y - p0.y;
                NORMALIZE2F_OVER_ZERO(dx, dy);
                temp_normals[i0].x = dy;
                temp_normals[i0].y = -dx;
            }

            for (int i0 = points.size() - 1, i1 = 0; i1 < points.size(); i0 = i1++) {
                const vec2& n0 = temp_normals[i0];
                const vec2& n1 = temp_normals[i1];
                float dm_x = (n0.x + n1.x) * 0.5f;
                float dm_y = (n0.y + n1.y) * 0.5f;
                FIXNORMAL2F(dm_x, dm_y);
                dm_x *= AA_SIZE * 0.5f;
                dm_y *= AA_SIZE * 0.5f;

                color clr;
                float t = math::clamp((points[i1] - min).dot(max - min) * (1.0f / (max - min).length_sqr()), 0.0f, 1.0f);
                clr = first_color + (second_color - first_color) * t;

                _vtx_write_ptr[0].pos.x = (points[i1].x - dm_x); _vtx_write_ptr[0].pos.y = (points[i1].y - dm_y); _vtx_write_ptr[0].uv = uv; _vtx_write_ptr[0].clr = clr;
                _vtx_write_ptr[1].pos.x = (points[i1].x + dm_x); _vtx_write_ptr[1].pos.y = (points[i1].y + dm_y); _vtx_write_ptr[1].uv = uv; _vtx_write_ptr[1].clr = color(clr, 0.f);
                _vtx_write_ptr += 2;

                _idx_write_ptr[0] = (unsigned short)(vtx_inner_idx + (i1 << 1)); _idx_write_ptr[1] = (unsigned short)(vtx_inner_idx + (i0 << 1)); _idx_write_ptr[2] = (unsigned short)(vtx_outer_idx + (i0 << 1));
                _idx_write_ptr[3] = (unsigned short)(vtx_outer_idx + (i0 << 1)); _idx_write_ptr[4] = (unsigned short)(vtx_outer_idx + (i1 << 1)); _idx_write_ptr[5] = (unsigned short)(vtx_inner_idx + (i1 << 1));
                _idx_write_ptr += 6;
            }
            _vtx_current_idx += (unsigned short)vtx_count;
        } else {
            const int idx_count = (points.size() - 2) * 3;
            const int vtx_count = points.size();
            prim_reserve(idx_count, vtx_count);
            for (int i = 0; i < vtx_count; i++) {
                color clr;
                float t = math::clamp((points[i] - min).dot(vec2(max.x, 0.f) - min) * (1.0f / (vec2(max.x, 0.f) - min).length_sqr()), 0.0f, 1.0f);
                clr = first_color + (second_color - first_color) * t;

                _vtx_write_ptr[0].pos = points[i]; _vtx_write_ptr[0].uv = uv; _vtx_write_ptr[0].clr = clr;
                _vtx_write_ptr++;
            }

            for (int i = 2; i < points.size(); i++) {
                _idx_write_ptr[0] = (unsigned short)(_vtx_current_idx); _idx_write_ptr[1] = (unsigned short)(_vtx_current_idx + i - 1); _idx_write_ptr[2] = (unsigned short)(_vtx_current_idx + i);
                _idx_write_ptr += 3;
            }
            _vtx_current_idx += (unsigned short)vtx_count;
        }
    }

    void draw_list::draw_bezier_curve(std::array<vec2, 4> points, color clr, float thickness, int num_segments) {
        if (clr.a() == 0.f) return;

        path_line_to(points[0]);
        path_bezier_curve_to({ points[1], points[2], points[3] }, num_segments);
        path_stroke(clr, false, thickness);
    }

    void draw_list::draw_image(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min, vec2 uv_max, color clr) {
        if (clr.a() == 0.f) return;

        const bool _push_texture_id = user_texture_id != _cmd_header.texture_id;
        if (_push_texture_id)
            push_texture_id(user_texture_id);

        prim_reserve(6, 4);
        prim_rect_uv(min, max, uv_min, uv_max, clr);

        if (_push_texture_id)
            pop_texture_id();
    }

    void draw_list::draw_image_quad(void* user_texture_id, std::array<vec2, 4> points, std::array<vec2, 4> uvs, color clr) {
        if (clr.a() == 0.f) return;

        const bool _push_texture_id = user_texture_id != _cmd_header.texture_id;
        if (_push_texture_id)
            push_texture_id(user_texture_id);

        prim_reserve(6, 4);
        prim_quad_uv(points, uvs, clr);

        if (_push_texture_id)
            pop_texture_id();
    }

    void draw_list::draw_image_rounded(void* user_texture_id, vec2 min, vec2 max, vec2 uv_min, vec2 uv_max, color clr, float rounding, flags_list<corner_flags> rounding_corners) {
        if (clr.a() == 0.f) return;

        if (rounding <= 0.0f || rounding_corners.contains(corner_flags::all)) {
            draw_image(user_texture_id, min, max, uv_min, uv_max, clr);
            return;
        }

        const bool _push_texture_id = _texture_id_stack.empty() || user_texture_id != _texture_id_stack.back();
        if (_push_texture_id)
            push_texture_id(user_texture_id);

        int vert_start_idx = vtx_buffer.size();
        path_rect(min, max, rounding, rounding_corners);
        path_fill_convex(clr);
        int vert_end_idx = vtx_buffer.size();
        helpers::shade_verts_linear_uv(this, vert_start_idx, vert_end_idx, min, max, uv_min, uv_max, true);

        if (_push_texture_id)
            pop_texture_id();
    }

    void draw_list::path_arc_to(vec2 center, float radius, float a_min, float a_max, int num_segments) {
        if (radius == 0.0f) {
            _path.push_back(center);
            return;
        }

        _path.reserve(_path.size() + (num_segments + 1));
        for (int i = 0; i <= num_segments; i++) {
            const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
            _path.push_back(vec2(center.x + cosf(a) * radius, center.y + sinf(a) * radius));
        }
    }

    void draw_list::path_arc_to_fast(vec2 center, float radius, int a_min_of_12, int a_max_of_12) {
        if (radius == 0.0f || a_min_of_12 > a_max_of_12) {
            _path.push_back(center);
            return;
        }

        _path.reserve(_path.size() + (a_max_of_12 - a_min_of_12 + 1));
        for (int a = a_min_of_12; a <= a_max_of_12; a++) {
            const vec2& c = _data->arc_fast_vtx[a % ARRAYSIZE(_data->arc_fast_vtx)];
            _path.push_back(vec2(center.x + c.x * radius, center.y + c.y * radius));
        }
    }

    void draw_list::path_bezier_curve_to(std::array<vec2, 3> points, int num_segments) {
        vec2 first_point = _path.back();
        if (num_segments == 0) {
            path_bezier_to_casteljau(&_path, { first_point, points[0], points[1], points[2] }, _data->curve_tessellation_tol, 0);
        } else {
            float t_step = 1.0f / (float)num_segments;
            for (int i_step = 1; i_step <= num_segments; i_step++) {
                float u = 1.0f - (t_step * i_step);
                float w1 = u * u * u;
                float w2 = 3 * u * u * (t_step * i_step);
                float w3 = 3 * u * (t_step * i_step) * (t_step * i_step);
                float w4 = (t_step * i_step) * (t_step * i_step) * (t_step * i_step);
                _path.push_back(first_point * w1 + points[0] * w2 + points[1] * w3 + points[2] * w4);//vec2(w1 * first_point.x + w2 * points[0].x + w3 * points[1].x + w4 * points[2].x, w1 * first_point.y + w2 * points[0].y + w3 * points[1].y + w4 * points[2].y));
            }
        }
    }

    void draw_list::path_rect(vec2 a, vec2 b, float rounding, flags_list<corner_flags> rounding_corners) {
        if (rounding_corners.contains(corner_flags::all))
            rounding_corners.add({ corner_flags::top, corner_flags::left, corner_flags::bot, corner_flags::right });

        rounding = math::min(rounding, fabsf(b.x - a.x) * (rounding_corners.contains(corner_flags::top) || rounding_corners.contains(corner_flags::bot) ? 0.5f : 1.0f) - 1.0f);
        rounding = math::min(rounding, fabsf(b.y - a.y) * (rounding_corners.contains(corner_flags::left) || rounding_corners.contains(corner_flags::right) ? 0.5f : 1.0f) - 1.0f);

        if (rounding <= 0.0f || rounding_corners.empty()) {
            path_line_to(a);
            path_line_to(vec2(b.x, a.y));
            path_line_to(b);
            path_line_to(vec2(a.x, b.y));
        } else {
            float rounding_tl = rounding_corners.contains(corner_flags::top_left) || rounding_corners.contains(corner_flags::top) || rounding_corners.contains(corner_flags::left) ? rounding : 0.0f;
            float rounding_tr = rounding_corners.contains(corner_flags::top_right) || rounding_corners.contains(corner_flags::top) || rounding_corners.contains(corner_flags::right) ? rounding : 0.0f;
            float rounding_br = rounding_corners.contains(corner_flags::bot_right) || rounding_corners.contains(corner_flags::bot) || rounding_corners.contains(corner_flags::right) ? rounding : 0.0f;
            float rounding_bl = rounding_corners.contains(corner_flags::bot_left) || rounding_corners.contains(corner_flags::bot) || rounding_corners.contains(corner_flags::left) ? rounding : 0.0f;

            if (rounding_corners.contains(corner_flags::all)) {
                rounding_tl = rounding;
                rounding_tr = rounding;
                rounding_br = rounding;
                rounding_bl = rounding;
            }

            path_arc_to_fast(vec2(a.x + rounding_tl, a.y + rounding_tl), rounding_tl, 6, 9);
            path_arc_to_fast(vec2(b.x - rounding_tr, a.y + rounding_tr), rounding_tr, 9, 12);
            path_arc_to_fast(vec2(b.x - rounding_br, b.y - rounding_br), rounding_br, 0, 3);
            path_arc_to_fast(vec2(a.x + rounding_bl, b.y - rounding_bl), rounding_bl, 3, 6);
        }
    }

    void draw_list::path_bezier_to_casteljau(std::vector<vec2>* path, std::array<vec2, 4> points, float tess_tol, int level) {
        //float dx = x4 - x1;
        //float dy = y4 - y1;

        vec2 d = points[3] - points[0];

        float d2 = ((points[0].x - points[3].x) * d.y - (points[0].y - points[3].y) * d.x);
        float d3 = ((points[2].x - points[3].x) * d.y - (points[2].y - points[3].x) * d.x);
        d2 = (d2 >= 0) ? d2 : -d2;
        d3 = (d3 >= 0) ? d3 : -d3;
        if ((d2 + d3) * (d2 + d3) < tess_tol * (d.x * d.x + d.y * d.y)) {
            path->push_back(vec2(points[3].x, points[3].y));
        } else if (level < 10) {
            float x12 = (points[0].x + points[1].x) * 0.5f, y12 = (points[0].y + points[1].y) * 0.5f;
            float x23 = (points[1].x + points[2].x) * 0.5f, y23 = (points[1].y + points[2].y) * 0.5f;
            float x34 = (points[2].x + points[3].x) * 0.5f, y34 = (points[2].y + points[3].y) * 0.5f;
            float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
            float x234 = (x23 + x34) * 0.5f, y234 = (y23 + y34) * 0.5f;
            float x1234 = (x123 + x234) * 0.5f, y1234 = (y123 + y234) * 0.5f;
            path_bezier_to_casteljau(path, { vec2(points[0].x, points[0].y), vec2(x12, y12), vec2(x123, y123), vec2(x1234, y123) }, tess_tol, level + 1);
            path_bezier_to_casteljau(path, { vec2(x1234, y1234), vec2(x234, y234), vec2(x34, y34), vec2(points[3].x, points[3].y) }, tess_tol, level + 1);
        }
    }

    void draw_list::add_draw_cmd() {
        helpers::cmd draw_cmd;
        draw_cmd.clip_rect = _cmd_header.clip_rect;
        draw_cmd.texture_id = _cmd_header.texture_id;
        draw_cmd.vtx_offset = _cmd_header.vtx_offset;
        draw_cmd.idx_offset = idx_buffer.size();

        assert(draw_cmd.clip_rect.min.x <= draw_cmd.clip_rect.max.x && draw_cmd.clip_rect.min.y <= draw_cmd.clip_rect.max.y);

        cmd_buffer.push_back(draw_cmd);
    }

    draw_list* draw_list::clone_output() const {
        draw_list* dst = new draw_list(_data);
        dst->cmd_buffer = cmd_buffer;
        dst->idx_buffer = idx_buffer;
        dst->vtx_buffer = vtx_buffer;
        dst->flags = flags;
        return dst;
    }

    void draw_list::prim_reserve(int idx_count, int vtx_count) {
        if (sizeof(unsigned short) == 2 && (_vtx_current_idx + vtx_count >= (1 << 16)) && (flags.contains(draw_list_flags::allow_vtx_offset))) {
            _cmd_header.vtx_offset = vtx_buffer.size();
            _on_changed_vtx_offset();
        }

        helpers::cmd* draw_cmd = &cmd_buffer.data()[cmd_buffer.size() - 1];
        draw_cmd->elem_count += idx_count;

        int vtx_buffer_old_size = vtx_buffer.size();
        vtx_buffer.resize(vtx_buffer_old_size + vtx_count);
        _vtx_write_ptr = vtx_buffer.data() + vtx_buffer_old_size;

        int idx_buffer_old_size = idx_buffer.size();
        idx_buffer.resize(idx_buffer_old_size + idx_count);
        _idx_write_ptr = idx_buffer.data() + idx_buffer_old_size;
    }

    void draw_list::prim_unreserve(int idx_count, int vtx_count) {
        helpers::cmd* draw_cmd = &cmd_buffer.data()[cmd_buffer.size() - 1];
        draw_cmd->elem_count -= idx_count;
        vtx_buffer.resize(vtx_buffer.size() - vtx_count);
        idx_buffer.resize(idx_buffer.size() - idx_count);
    }

    void draw_list::prim_rect(vec2 a, vec2 c, color clr) {
        vec2 b(c.x, a.y), d(a.x, c.y), uv(_data->tex_uv_white_pixel);
        unsigned short idx = (unsigned short)_vtx_current_idx;
        _idx_write_ptr[0] = idx; _idx_write_ptr[1] = (unsigned short)(idx + 1); _idx_write_ptr[2] = (unsigned short)(idx + 2);
        _idx_write_ptr[3] = idx; _idx_write_ptr[4] = (unsigned short)(idx + 2); _idx_write_ptr[5] = (unsigned short)(idx + 3);
        _vtx_write_ptr[0].pos = a; _vtx_write_ptr[0].uv = uv; _vtx_write_ptr[0].clr = clr;
        _vtx_write_ptr[1].pos = b; _vtx_write_ptr[1].uv = uv; _vtx_write_ptr[1].clr = clr;
        _vtx_write_ptr[2].pos = c; _vtx_write_ptr[2].uv = uv; _vtx_write_ptr[2].clr = clr;
        _vtx_write_ptr[3].pos = d; _vtx_write_ptr[3].uv = uv; _vtx_write_ptr[3].clr = clr;
        _vtx_write_ptr += 4;
        _vtx_current_idx += 4;
        _idx_write_ptr += 6;
    }

    void draw_list::prim_rect_uv(vec2 a, vec2 c, vec2 uv_a, vec2 uv_c, color clr) {
        vec2 b(c.x, a.y), d(a.x, c.y), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
        unsigned short idx = (unsigned short)_vtx_current_idx;
        _idx_write_ptr[0] = idx; _idx_write_ptr[1] = (unsigned short)(idx + 1); _idx_write_ptr[2] = (unsigned short)(idx + 2);
        _idx_write_ptr[3] = idx; _idx_write_ptr[4] = (unsigned short)(idx + 2); _idx_write_ptr[5] = (unsigned short)(idx + 3);
        _vtx_write_ptr[0].pos = a; _vtx_write_ptr[0].uv = uv_a; _vtx_write_ptr[0].clr = clr;
        _vtx_write_ptr[1].pos = b; _vtx_write_ptr[1].uv = uv_b; _vtx_write_ptr[1].clr = clr;
        _vtx_write_ptr[2].pos = c; _vtx_write_ptr[2].uv = uv_c; _vtx_write_ptr[2].clr = clr;
        _vtx_write_ptr[3].pos = d; _vtx_write_ptr[3].uv = uv_d; _vtx_write_ptr[3].clr = clr;
        _vtx_write_ptr += 4;
        _vtx_current_idx += 4;
        _idx_write_ptr += 6;
    }

    void draw_list::prim_quad_uv(std::array<vec2, 4> points, std::array<vec2, 4> uvs, color clr) {
        unsigned short idx = (unsigned short)_vtx_current_idx;
        _idx_write_ptr[0] = idx; _idx_write_ptr[1] = (unsigned short)(idx + 1); _idx_write_ptr[2] = (unsigned short)(idx + 2);
        _idx_write_ptr[3] = idx; _idx_write_ptr[4] = (unsigned short)(idx + 2); _idx_write_ptr[5] = (unsigned short)(idx + 3);
        _vtx_write_ptr[0].pos = uvs[0]; _vtx_write_ptr[0].uv = uvs[0]; _vtx_write_ptr[0].clr = clr;
        _vtx_write_ptr[1].pos = uvs[1]; _vtx_write_ptr[1].uv = uvs[1]; _vtx_write_ptr[1].clr = clr;
        _vtx_write_ptr[2].pos = uvs[2]; _vtx_write_ptr[2].uv = uvs[2]; _vtx_write_ptr[2].clr = clr;
        _vtx_write_ptr[3].pos = uvs[3]; _vtx_write_ptr[3].uv = uvs[3]; _vtx_write_ptr[3].clr = clr;
        _vtx_write_ptr += 4;
        _vtx_current_idx += 4;
        _idx_write_ptr += 6;
    }

    void draw_list::_reset_for_begin_render() {
        assert(offsetof(helpers::cmd, clip_rect) == 0);
        assert(offsetof(helpers::cmd, texture_id) == sizeof(rect));
        assert(offsetof(helpers::cmd, vtx_offset) == sizeof(rect) + sizeof(void*));

        cmd_buffer.resize(0);
        idx_buffer.resize(0);
        vtx_buffer.resize(0);
        flags = _data->initial_flags;
        memset(&_cmd_header, 0, sizeof(_cmd_header));
        _vtx_current_idx = 0;
        _vtx_write_ptr = NULL;
        _idx_write_ptr = NULL;
        _clip_rect_stack.resize(0);
        _texture_id_stack.resize(0);
        _path.resize(0);
        cmd_buffer.push_back(helpers::cmd());
    }

    void draw_list::_clear_free_memory() {
        cmd_buffer.clear();
        idx_buffer.clear();
        vtx_buffer.clear();
        _vtx_current_idx = 0;
        _vtx_write_ptr = NULL;
        _idx_write_ptr = NULL;
        _clip_rect_stack.clear();
        _texture_id_stack.clear();
        _path.clear();
    }

    void draw_list::_pop_unused_draw_cmd() {
        if (cmd_buffer.size() == 0) return;
        helpers::cmd* curr_cmd = &cmd_buffer.data()[cmd_buffer.size() - 1];
        if (curr_cmd->elem_count == 0)
            cmd_buffer.pop_back();
    }

    void draw_list::_on_changed_clip_rect() {
        helpers::cmd* curr_cmd = &cmd_buffer.data()[cmd_buffer.size() - 1];
        if (curr_cmd->elem_count != 0 && memcmp(&curr_cmd->clip_rect, &_cmd_header.clip_rect, sizeof(rect)) != 0) {
            add_draw_cmd();
            return;
        }

        helpers::cmd* prev_cmd = curr_cmd - 1;
        if (curr_cmd->elem_count == 0 && cmd_buffer.size() > 1 && memcmp(&_cmd_header, prev_cmd, offsetof(helpers::cmd, vtx_offset) + sizeof(unsigned int)) == 0) {
            cmd_buffer.pop_back();
            return;
        }

        curr_cmd->clip_rect = _cmd_header.clip_rect;
    }

    void draw_list::_on_changed_texture_id() {
        helpers::cmd* curr_cmd = &cmd_buffer.data()[cmd_buffer.size() - 1];
        if (curr_cmd->elem_count != 0 && curr_cmd->texture_id != _cmd_header.texture_id) {
            add_draw_cmd();
            return;
        }

        helpers::cmd* prev_cmd = curr_cmd - 1;
        if (curr_cmd->elem_count == 0 && cmd_buffer.size() > 1 && memcmp(&_cmd_header, prev_cmd, offsetof(helpers::cmd, vtx_offset) + sizeof(unsigned int)) == 0) {
            cmd_buffer.pop_back();
            return;
        }

        curr_cmd->texture_id = _cmd_header.texture_id;
    }

    void draw_list::_on_changed_vtx_offset() {
        _vtx_current_idx = 0;
        helpers::cmd* curr_cmd = &cmd_buffer.data()[cmd_buffer.size() - 1];
        if (curr_cmd->elem_count != 0) {
            add_draw_cmd();
            return;
        }
        curr_cmd->vtx_offset = _cmd_header.vtx_offset;
    }

    void shutdown() {
        if (null_font::vars::font_atlas && null_font::vars::atlas_owned_by_initialize) {
            null_font::vars::font_atlas->locked = false;
            null_font::vars::font_atlas->~atlas();
            free(null_font::vars::font_atlas);
        }

        null_font::vars::font_atlas = NULL;

        if (!settings::initialized) return;

        null_font::vars::font_list.clear();
        data_builder.clear_free_memory();
        background_draw_list._clear_free_memory();
        foreground_draw_list._clear_free_memory();

        settings::initialized = false;
    }

    void begin_render(HWND hwnd) {
        RECT display_rect = { 0, 0, 0, 0 };
        ::GetClientRect(hwnd, &display_rect);
        settings::display_size = vec2((float)(display_rect.right - display_rect.left), (float)(display_rect.bottom - display_rect.top));

        null_font::vars::font_atlas->locked = true;
        null_font::set_current_font(null_font::get_default_font());
        assert(null_font::vars::main_font->is_loaded());
        shared_data.clip_rect_fullscreen = rect(0.0f, 0.0f, settings::display_size.x, settings::display_size.y);
        shared_data.curve_tessellation_tol = 1.25f;
        shared_data.set_circle_segment_max_error(1.60f);
        shared_data.initial_flags.add({ draw_list_flags::anti_aliased_lines, draw_list_flags::anti_aliased_lines_use_tex, draw_list_flags::anti_aliased_fill });
        if (settings::renderer_has_vtx_offset) shared_data.initial_flags.add(draw_list_flags::allow_vtx_offset);

        background_draw_list._reset_for_begin_render();
        background_draw_list.push_texture_id(null_font::vars::font_atlas->tex_id);
        background_draw_list.push_clip_rect_full_screen();

        foreground_draw_list._reset_for_begin_render();
        foreground_draw_list.push_texture_id(null_font::vars::font_atlas->tex_id);
        foreground_draw_list.push_clip_rect_full_screen();

        for (null_gui::window* wnd : null_gui::deeps::windows) {
            wnd->draw_list->_reset_for_begin_render();
            wnd->draw_list->push_texture_id(null_font::vars::font_atlas->tex_id);
            wnd->draw_list->push_clip_rect_full_screen();
        }

        draw_data.clear();
    }

    void render() {
        data_builder.clear();

        if (!background_draw_list.vtx_buffer.empty())
            helpers::add_draw_list_to_draw_data(&data_builder.layer, &background_draw_list);

        for (null_gui::window* wnd : null_gui::deeps::windows) {
            if (!wnd->flags.contains(null_gui::window_flags::group) && !wnd->draw_list->vtx_buffer.empty()) helpers::add_draw_list_to_draw_data(&data_builder.layer, wnd->draw_list);
        }

        if (!foreground_draw_list.vtx_buffer.empty())
            helpers::add_draw_list_to_draw_data(&data_builder.layer, &foreground_draw_list);

        helpers::setup_draw_data(&data_builder.layer, &draw_data);
    }
}