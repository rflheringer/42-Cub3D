/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:18:49 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/11 18:29:51 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"
#include <stdint.h>

#define HEART_W 11
#define HEART_H 9

/* Máscara do coração (1 = preenchido, 0 = vazio) */
static const uint8_t	g_heart_mask[HEART_H][HEART_W] = {
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,1,1,1,1,0,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,0,0,1,1,1,1,1,0,0,0},
    {0,0,0,0,1,1,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0}
};

static void	put_rect(mlx_image_t *img, int x, int y, int w, int h, uint32_t c)
{
    int	i;
    int	j;

    if (!img || w <= 0 || h <= 0)
        return ;
    j = 0;
    while (j < h)
    {
        i = 0;
        while (i < w)
        {
            if (x + i >= 0 && x + i < (int)img->width
                && y + j >= 0 && y + j < (int)img->height)
                mlx_put_pixel(img, x + i, y + j, c);
            i++;
        }
        j++;
    }
}

static int	has_fill_neighbor(int i, int j)
{
    int	di;
    int	dj;
    int	ii;
    int	jj;

    dj = -1;
    while (dj <= 1)
    {
        di = -1;
        while (di <= 1)
        {
            if (!(di == 0 && dj == 0))
            {
                ii = i + di;
                jj = j + dj;
                if (ii >= 0 && ii < HEART_W && jj >= 0 && jj < HEART_H)
                {
                    if (g_heart_mask[jj][ii] == 1)
                        return (1);
                }
            }
            di++;
        }
        dj++;
    }
    return (0);
}

/* Desenha um coração em (x,y) com escala "s" (cada célula da máscara = s x s). 
   filled != 0 desenha preenchido de vermelho + contorno preto.
   filled == 0 desenha apenas o contorno. */
static void	draw_heart_icon(mlx_image_t *img, int x, int y, int s, int filled)
{
    int	i;
    int	j;
    int	cx;
    int	cy;

    /* Contorno (pixels vizinhos ao preenchido) */
    j = 0;
    while (j < HEART_H)
    {
        i = 0;
        while (i < HEART_W)
        {
            if (g_heart_mask[j][i] == 0 && has_fill_neighbor(i, j))
            {
				if (!(j == 0 && (i == 0 || i == 5 || i == HEART_W - 1)))
                {
					cx = x + i * s;
					cy = y + j * s;
                	put_rect(img, cx, cy, s, s, 0x000000FF);
				}
            }
            i++;
        }
        j++;
    }
    /* Preenchimento vermelho (se cheio) */
    if (filled)
    {
        j = 0;
        while (j < HEART_H)
        {
            i = 0;
            while (i < HEART_W)
            {
                if (g_heart_mask[j][i] == 1)
                {
                    cx = x + i * s;
                    cy = y + j * s;
                    put_rect(img, cx, cy, s, s, 0xFF0000FF);
                }
                i++;
            }
            j++;
        }
    }
}

void	draw_life(t_game *game)
{
    mlx_image_t	*img;
    int			target_h;
    int			margin;
    int			gap;
    int			scale_cell; /* escala por célula do coração */
    int			heart_w;
    int			heart_h;
    int			total_h;
    int			filled;
    int			i;
    int			x;
    int			y0;

    if (!game || !game->player || !game->raycasting
        || !game->raycasting->image)
        return ;
    img = game->raycasting->image;

    /* Área alvo no lado direito (40% da altura da tela) */
    target_h = (int)(HEIGHT * 0.40);
    if (target_h < 120)
        target_h = 120;
    margin = 14;
    gap = 4;

    /* Calcula escala por célula para caber LIFE_MAX corações em target_h */
    scale_cell = (target_h - gap * (LIFE_MAX - 1)) / (HEART_H * LIFE_MAX);
    if (scale_cell < 1)
        scale_cell = 1;

    heart_w = HEART_W * scale_cell;
    heart_h = HEART_H * scale_cell;
    total_h = heart_h * LIFE_MAX + gap * (LIFE_MAX - 1);

    /* Centraliza verticalmente na área alvo */
    y0 = margin + (target_h - total_h) / 2;
    x = WIDTH - heart_w - (margin + 2);

    /* Fundo sutil atrás dos corações */
    //put_rect(img, x - 4, y0 - 4, heart_w + 8, total_h + 8, 0x000000A0);

    filled = game->player->hp;
    if (filled < 0)
        filled = 0;
    if (filled > LIFE_MAX)
        filled = LIFE_MAX;

    i = 0;
    while (i < LIFE_MAX)
    {
        int iy = y0 + i * (heart_h + gap);
        /* Contorno sempre; preenchido apenas se i < hp */
        draw_heart_icon(img, x, iy, scale_cell, (i < filled));
        i++;
    }
}
