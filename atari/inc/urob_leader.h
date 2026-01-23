// ====== one character sequences ======
// row 1 starting 0
ZMK_LEADER_SEQUENCE(u_q,      &u_q,         0)
ZMK_LEADER_SEQUENCE(u_e,      &u_e,         2)
ZMK_LEADER_SEQUENCE(u_r,      &u_r,         3)
ZMK_LEADER_SEQUENCE(u_t,      &u_t,         4)
ZMK_LEADER_SEQUENCE(u_y,      &u_y,         5)
ZMK_LEADER_SEQUENCE(u_u,      &u_u,         6)
ZMK_LEADER_SEQUENCE(u_i,      &u_i,         7)
ZMK_LEADER_SEQUENCE(u_p,      &u_p,         9)

// row 2 starting 11
ZMK_LEADER_SEQUENCE(u_a,      &u_a,         11)
ZMK_LEADER_SEQUENCE(u_s,      &u_s,         12)
ZMK_LEADER_SEQUENCE(u_d,      &u_d,         13)
ZMK_LEADER_SEQUENCE(u_f,      &u_f,         14)
ZMK_LEADER_SEQUENCE(u_h,      &u_h,         16)
ZMK_LEADER_SEQUENCE(u_j,      &u_j,         17)
ZMK_LEADER_SEQUENCE(u_k,      &u_k,         18)
ZMK_LEADER_SEQUENCE(u_l,      &u_l,         19)

// row 3 starting 21
ZMK_LEADER_SEQUENCE(u_z,      &u_z,         21)
ZMK_LEADER_SEQUENCE(u_x,      &u_x,         22)
ZMK_LEADER_SEQUENCE(u_c,      &u_c,         23)
ZMK_LEADER_SEQUENCE(u_v,      &u_v,         24)
ZMK_LEADER_SEQUENCE(u_b,      &u_b,         25)
ZMK_LEADER_SEQUENCE(u_n,      &u_n,         26)
ZMK_LEADER_SEQUENCE(u_m,      &u_m,         27)
ZMK_LEADER_SEQUENCE(u_apo,    &u_apo,       30)

// row 4 starting 31
ZMK_LEADER_SEQUENCE(u_lft,    &u_lft,       35)
ZMK_LEADER_SEQUENCE(u_rgt,    &u_rgt,       37)

// ====== two character sequences ======
// mongo commands M
/*
f  field subquery
l  locationId query
a  and subquery
r  regex subquery
t  technicalProvider query
n  not equal subquery
e  exists query
o  or subquery
c  country query
*/  
ZMK_LEADER_SEQUENCE(u_mf,      &u_mf,         27 14)
ZMK_LEADER_SEQUENCE(u_ml,      &u_ml,         27 19)
ZMK_LEADER_SEQUENCE(u_ma,      &u_ma,         27 11)
ZMK_LEADER_SEQUENCE(u_mr,      &u_mr,         27 3)
ZMK_LEADER_SEQUENCE(u_mt,      &u_mt,         27 4)
ZMK_LEADER_SEQUENCE(u_mn,      &u_mn,         27 26)
ZMK_LEADER_SEQUENCE(u_me,      &u_me,         27 2)
ZMK_LEADER_SEQUENCE(u_mo,      &u_mo,         27 8)
ZMK_LEADER_SEQUENCE(u_mc,      &u_mc,         27 23)

// status messages S
/*
m  good morning
f  feierabend
l  lunch
b  back
a  afk
w  wochenende
u  urlaub
*/  
ZMK_LEADER_SEQUENCE(u_sw,      &u_sw,         12 1)
ZMK_LEADER_SEQUENCE(u_sf,      &u_sf,         12 14)
ZMK_LEADER_SEQUENCE(u_sb,      &u_sb,         12 25)
ZMK_LEADER_SEQUENCE(u_sl,      &u_sl,         12 19)
ZMK_LEADER_SEQUENCE(u_su,      &u_su,         12 6)
ZMK_LEADER_SEQUENCE(u_sa,      &u_sa,         12 11)
ZMK_LEADER_SEQUENCE(u_sm,      &u_sm,         12 27)

// names N
/*
r  René
S  Sebastian
j  Josias
b  Barbara
k  Saskia
c  Christoph
d  Daniel
f  Florian
p  Philipp
*/  
ZMK_LEADER_SEQUENCE(u_nf,      &u_nf,         26 14)
ZMK_LEADER_SEQUENCE(u_np,      &u_np,         26 9)
ZMK_LEADER_SEQUENCE(u_nb,      &u_nb,         26 25)
ZMK_LEADER_SEQUENCE(u_nj,      &u_nj,         26 17)
ZMK_LEADER_SEQUENCE(u_nr,      &u_nr,         26 3)
ZMK_LEADER_SEQUENCE(u_ns,      &u_ns,         26 12)
ZMK_LEADER_SEQUENCE(u_nc,      &u_nc,         26 23)
ZMK_LEADER_SEQUENCE(u_nd,      &u_nd,         26 13)
ZMK_LEADER_SEQUENCE(u_nk,      &u_nk,         26 18)

// text T
/*
a  availability
c  connector
d  dynamic
h  evse
l  location
p  provider
s  station
z  Azure
m  Guten Morgen
k  Kubernetes
*/  
ZMK_LEADER_SEQUENCE(u_tp,      &u_tp,         4 9)
ZMK_LEADER_SEQUENCE(u_tl,      &u_tl,         4 19)
ZMK_LEADER_SEQUENCE(u_ta,      &u_ta,         4 11)
ZMK_LEADER_SEQUENCE(u_ts,      &u_ts,         4 12)
ZMK_LEADER_SEQUENCE(u_tm,      &u_tm,         4 27)
ZMK_LEADER_SEQUENCE(u_te,      &u_te,         4 2)
ZMK_LEADER_SEQUENCE(u_tz,      &u_tz,         4 21)
ZMK_LEADER_SEQUENCE(u_tc,      &u_tc,         4 23)
ZMK_LEADER_SEQUENCE(u_td,      &u_td,         4 13)
ZMK_LEADER_SEQUENCE(u_tk,      &u_tk,         4 18)

// emojis E
/*
C  :coffeezombie:
P  :puzzled:
T  :think:
F  :this-is-fine-fire:
R  :facepalmm:
S  :man-shrugging:
M  :machete:
H  :homerdisappear:
Y  :homer_scream:
K  :(
V  :)
D  :D
O  :+1:
W  :sweat_smile:
*/  
ZMK_LEADER_SEQUENCE(u_ew,      &u_ew,         2 1)
ZMK_LEADER_SEQUENCE(u_ef,      &u_ef,         2 14)
ZMK_LEADER_SEQUENCE(u_ep,      &u_ep,         2 9)
ZMK_LEADER_SEQUENCE(u_ey,      &u_ey,         2 5)
ZMK_LEADER_SEQUENCE(u_er,      &u_er,         2 3)
ZMK_LEADER_SEQUENCE(u_es,      &u_es,         2 12)
ZMK_LEADER_SEQUENCE(u_et,      &u_et,         2 4)
ZMK_LEADER_SEQUENCE(u_em,      &u_em,         2 27)
ZMK_LEADER_SEQUENCE(u_eo,      &u_eo,         2 8)
ZMK_LEADER_SEQUENCE(u_ec,      &u_ec,         2 23)
ZMK_LEADER_SEQUENCE(u_ed,      &u_ed,         2 13)
ZMK_LEADER_SEQUENCE(u_eb,      &u_eb,         2 24)
ZMK_LEADER_SEQUENCE(u_en,      &u_en,         2 18)
ZMK_LEADER_SEQUENCE(u_eh,      &u_eh,         2 16)

// code snippets P
/*
i  if template
w  when template
e  enum class template
d  data class template
l  logger template
*/  
ZMK_LEADER_SEQUENCE(u_pw,      &u_pw,         9 1)
ZMK_LEADER_SEQUENCE(u_pl,      &u_pl,         9 19)
ZMK_LEADER_SEQUENCE(u_pe,      &u_pe,         9 2)
ZMK_LEADER_SEQUENCE(u_pi,      &u_pi,         9 7)
ZMK_LEADER_SEQUENCE(u_pd,      &u_pd,         9 13)

// execute programs X
/*
p  Powerpoint
n  Notepad++
w  Word
o  OpenLens
f  Firefox
m  Outlook
e  Explorer
c  Calc
a  Admin console
x  console
*/  
ZMK_LEADER_SEQUENCE(u_xw,      &u_xw,         22 1)
ZMK_LEADER_SEQUENCE(u_xf,      &u_xf,         22 14)
ZMK_LEADER_SEQUENCE(u_xp,      &u_xp,         22 9)
ZMK_LEADER_SEQUENCE(u_xa,      &u_xa,         22 11)
ZMK_LEADER_SEQUENCE(u_xm,      &u_xm,         22 27)
ZMK_LEADER_SEQUENCE(u_xn,      &u_xn,         22 26)
ZMK_LEADER_SEQUENCE(u_xe,      &u_xe,         22 2)
ZMK_LEADER_SEQUENCE(u_xo,      &u_xo,         22 8)
ZMK_LEADER_SEQUENCE(u_xx,      &u_xx,         22 22)
ZMK_LEADER_SEQUENCE(u_xc,      &u_xc,         22 23)
