<script lang="ts">
	import { start, end } from '../../store/store';
	import { onMount } from 'svelte';

	let startTime = '';
	let endTime = '';
	let plotContainer: HTMLElement;

	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));

	let plotly: null | any;
	$: plotly = null;
	$: contoursPlot(plotly, startTime, endTime);

	onMount(async () => {
		plotly = await import('plotly.js-dist-min');
	});

	async function contoursPlot(plotly: any, startTime: string, endTime: string) {
		if (!plotly) return;

		const response = await fetch(
			`http://127.0.0.1:8000/contours?startTime=${startTime}&endTime=${endTime}`
		);
		const data = await response.json();

		const trace1: Plotly.Data = {
			x: data.dates.map((date: string, index: number) => {
                console.log(date)
                const [year, month, dayAndTime] = date.split('-');
                const [day, time] = dayAndTime.split('_');
                const hour = time.slice(0, 2)
                const minute = time.slice(2, 4)
                return new Date(+year, +month, +day, +hour, +minute).getMinutes()
            }),
			y: data.contourArea,
			line: { color: 'white' },
            fillcolor: 'white',
			type: 'bar',
            marker: {
                color: 'rgba(13, 195, 16, 0.9)'
            }
		};

		const plotlyData: Plotly.Data[] = [trace1];

		plotly.newPlot(plotContainer, plotlyData, {
			font: {
                family: '"open-sans", sans-serif',
                size: 12,
                color: 'rgba(0, 0, 0, 0.9)'
			},
			plot_bgcolor: 'white',
			paper_bgcolor: 'white',
            xaxis: {
                title: '<b>Time in minutes</b>',
                color: 'rgba(0, 0, 0, 0.6)'
            },
            yaxis: {
                title: '<b>Area of movement</b>',
                color: 'rgba(0, 0, 0, 0.6)'
            },
            title: {
                text: '<b>Contour area</b>'
            }
		}, {
            displayModeBar: false,
            displaylogo: false,
            responsive: true
        });
	}
</script>

<div bind:this={plotContainer}><!-- Plotly chart will be drawn inside this DIV --></div>