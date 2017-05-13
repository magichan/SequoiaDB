let SessionLoad = 1
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/project/SequoiaDB/SequoiaDB/engine
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +244 pmd/pmdMain.cpp
badd +523 pmd/fmpController.cpp
badd +142 include/pmd.hpp
badd +172 coord/coordCB.cpp
badd +42 include/ossErr.h
badd +156 include/pdTrace.hpp
badd +87 include/ossUtil.h
badd +31 pcre/pcre_internal.h
badd +114 pd/pdTrace.cpp
badd +305 include/ossAtomic.hpp
badd +146 pmd/pmd.cpp
badd +127 pd/pd.cpp
badd +334 pmd/pmdEnv.cpp
badd +54 include/pmdEDU.hpp
badd +117 include/pmdEnv.hpp
badd +482 include/msgDef.h
badd +313 include/msg.h
badd +575 oss/ossEDU.cpp
badd +113 include/pd.hpp
badd +299 pmd/pmdController.cpp
badd +122 include/pmdController.hpp
badd +166 include/ossSocket.hpp
badd +41 include/sdbInterface.hpp
badd +92 include/ossTypes.h
argglobal
silent! argdel *
edit pmd/pmd.cpp
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd _ | wincmd |
split
1wincmd k
wincmd w
set nosplitbelow
set nosplitright
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 32 + 95) / 190)
exe '2resize ' . ((&lines * 21 + 23) / 46)
exe 'vert 2resize ' . ((&columns * 157 + 95) / 190)
exe '3resize ' . ((&lines * 22 + 23) / 46)
exe 'vert 3resize ' . ((&columns * 157 + 95) / 190)
argglobal
enew
file __Tag_List__
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=9999
setlocal fml=0
setlocal fdn=20
setlocal fen
lcd ~/project/SequoiaDB/SequoiaDB/engine
wincmd w
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 147 - ((13 * winheight(0) + 10) / 21)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
147
normal! 0
lcd ~/project/SequoiaDB/SequoiaDB/engine
wincmd w
argglobal
edit ~/project/SequoiaDB/SequoiaDB/engine/pmd/pmdMain.cpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 233 - ((10 * winheight(0) + 11) / 22)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
233
normal! 065|
lcd ~/project/SequoiaDB/SequoiaDB/engine
wincmd w
3wincmd w
exe 'vert 1resize ' . ((&columns * 32 + 95) / 190)
exe '2resize ' . ((&lines * 21 + 23) / 46)
exe 'vert 2resize ' . ((&columns * 157 + 95) / 190)
exe '3resize ' . ((&lines * 22 + 23) / 46)
exe 'vert 3resize ' . ((&columns * 157 + 95) / 190)
tabnext 1
if exists('s:wipebuf') && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 winminheight=1 winminwidth=1 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
