" Plugins will be downloaded under the specified directory.
set nocompatible
set t_Co=256
syntax enable
colorscheme sublimemonokai
set number

set ai
set tabstop=4
set shiftwidth=4
set cursorline
set cursorcolumn
color better-molokai

set statusline=
set statusline+=\ %F\ %M\ %Y\ %R " Status line left side.
set statusline+=%=  " Use a divider to separate the left side from the right side.
set statusline+=\ ascii:\ %b\ hex:\ 0x%B\ row:\ %l\ col:\ %c\ percent:\ %p%%  " Status line right side.
set laststatus=2 " Show the status on the second to last line.
set cursorline
set cursorcolumn

let g:sublimemonokai_term_italic = 1

let g:airline_powerline_fonts = 1 "Включить поддержку Powerline шрифтов
let g:airline#extensions#keymap#enabled = 0 "Не показывать текущий маппинг
let g:airline_section_z = "\ue0a1:%l/%L Col:%c" "Кастомная графа положения курсора
let g:Powerline_symbols='unicode' "Поддержка unicode
let g:airline#extensions#xkblayout#enabled = 0 "Про это позже расскажу

call plug#begin(has('nvim') ? stdpath('data') . '/plugged' : '~/.vim/plugged')

" Declare the list of plugins.
"Plug 'tpope/vim-sensible'
"Plug 'junegunn/seoul256.vim'
"Plug 'crusoexia/vim-monokai'
Plug 'erichdongubler/vim-sublime-monokai'
Plug 'vim-airline/vim-airline'
" List ends here. Plugins become visible to Vim after this call.
call plug#end()

