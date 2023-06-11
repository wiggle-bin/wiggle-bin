<script lang="ts">
	import { onMount } from 'svelte';

    export let trace: Plotly.Data | undefined;
    export let title: String;
    export let xaxisTitle: String;
    export let yaxisTitle: String;

	let plotContainer: HTMLElement;

	let plotly: null | any;
	$: plotly = null;
	$: plot(plotly, trace);

	onMount(async () => {
		plotly = await import('plotly.js-dist-min');
	});

	async function plot(plotly: any, trace: any) {
		if (!plotly) return;

		const plotlyData: Plotly.Data[] = [trace];

		plotly.newPlot(plotContainer, plotlyData, {
			font: {
                family: '"open-sans", sans-serif',
                size: 12,
                color: 'black'
			},
			plot_bgcolor: 'white',
			paper_bgcolor: 'white',
            xaxis: {
                title: `<b>${xaxisTitle}</b>`,
                color: 'black'
            },
            yaxis: {
                title: `<b>${yaxisTitle}</b>`,
                color: 'black'
            },
            title: {
                text: `<b>${title}</b>`
            }
		}, {
            displayModeBar: false,
            displaylogo: false,
            responsive: true
        });
	}
</script>

<div bind:this={plotContainer}><!-- Plotly chart will be drawn inside this DIV --></div>